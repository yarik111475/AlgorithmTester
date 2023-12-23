#include "BaseAlgorithm.h"

void BaseAlgorithm::run()
{
    try{
        if(dataOrigin_.isEmpty()){
            const QString lastError {"Origin data is empty!"};
            emit finishedSignal(lastError);
            goto cleanup;
            processSmooth();
            processRCCR2();
            processFilter();
        }
    }
    catch(const std::exception& ex){
        const QString lastError {ex.what()};
        emit finishedSignal(lastError);
        goto cleanup;
    }
    catch(...){
        const QString lastError {"Unknown exception"};
        emit finishedSignal(lastError);
        goto cleanup;
    }
    emit finishedSignal(QString::Null());
cleanup:
    QThread::quit();
}

BaseAlgorithm::BaseAlgorithm(const QVector<double> &dataOrigin, QObject *parent)
    :QThread{parent},dataOrigin_{dataOrigin}
{ 
}

BaseAlgorithm::~BaseAlgorithm()
{
    QThread::quit();
    QThread::wait();
}
