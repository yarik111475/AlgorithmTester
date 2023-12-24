#include "BaseAlgorithm.h"
#include <QDebug>

void BaseAlgorithm::run()
{
    try{
        if(dataOrigin_.isEmpty()){
            const QString lastError {"Origin data is empty!"};
            emit finishedSignal(lastError);
            QThread::quit();
        }
        processStepOne();
        processStepTwo();
        processStepThree();
    }
    catch(const std::exception& ex){
        const QString lastError {ex.what()};
        emit finishedSignal(lastError);
        QThread::quit();
    }
    catch(...){
        const QString lastError {"Unknown exception"};
        emit finishedSignal(lastError);
        QThread::quit();
    }
    emit finishedSignal(QString::Null());
    QThread::exec();
}

BaseAlgorithm::BaseAlgorithm(const QVector<double> &dataOrigin, double triggerThreshold, int span, int tau, QObject *parent)
    :QThread{parent},dataOrigin_{dataOrigin},triggerThreshold_{triggerThreshold},span_{span},tau_{tau}
{ 
}

BaseAlgorithm::~BaseAlgorithm()
{
    QThread::quit();
    QThread::wait();
}
