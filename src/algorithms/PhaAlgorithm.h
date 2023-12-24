#ifndef PHAALGORITHM_H
#define PHAALGORITHM_H

#include "BaseAlgorithm.h"
#include <QPair>

class PhaAlgorithm : public BaseAlgorithm
{
    Q_OBJECT
private:
    QPair<int,int> makeFilterParams();
    QVector<double> makeSmooth(const QVector<double>& data,int span);
    QVector<double> makeRCCR2(const QVector<double>& data);
protected:
    virtual void processStepOne()override;
    virtual void processStepTwo()override;
    virtual void processStepThree()override;
public:
    explicit PhaAlgorithm(const QVector<double>& dataOrigin,double triggerThreshold,int span,int tau,QObject *parent = nullptr);
    virtual ~PhaAlgorithm()=default;
    virtual QString getName()const override;
};

#endif // PHAALGORITHM_H
