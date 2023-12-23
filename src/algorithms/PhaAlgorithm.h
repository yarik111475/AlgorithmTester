#ifndef PHAALGORITHM_H
#define PHAALGORITHM_H

#include "BaseAlgorithm.h"

class PhaAlgorithm : public BaseAlgorithm
{
    Q_OBJECT
protected:
    virtual void processSmooth()override;
    virtual void processRCCR2()override;
    virtual void processFilter()override;
public:
    explicit PhaAlgorithm(const QVector<double>& dataOrigin,QObject *parent = nullptr);
    virtual ~PhaAlgorithm()=default;
    virtual QString getName()const override;
};

#endif // PHAALGORITHM_H
