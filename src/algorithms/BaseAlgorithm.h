#ifndef BASEALGORITHM_H
#define BASEALGORITHM_H

#include <QThread>
#include <QVector>
#include <QString>

class BaseAlgorithm : public QThread
{
    Q_OBJECT
protected:
    double triggerThreshold_ {};
    int span_ {};
    int tau_ {};

    QVector<double> dataOrigin_ {};
    virtual void run()override;
    virtual void processStepOne()=0;
    virtual void processStepTwo()=0;
    virtual void processStepThree()=0;
public:
    explicit BaseAlgorithm(const QVector<double>& dataOrigin,double triggerThreshold,int span,int tau,QObject* parent=nullptr);
    virtual ~BaseAlgorithm();
    virtual QString getName()const=0;
signals:
    void endStepOneSignal(const QVector<double>& dataSmooth);
    void endStepTwoSignal(const QVector<double>& dataRCCR2);
    void endStepThreeSignal(const QVector<double>& dataFiltered);
    void finishedSignal(const QString& lastError);

};

#endif // BASEALGORITHM_H
