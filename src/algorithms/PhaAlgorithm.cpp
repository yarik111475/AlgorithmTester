#include "PhaAlgorithm.h"

void PhaAlgorithm::processSmooth()
{

}

void PhaAlgorithm::processRCCR2()
{

}

void PhaAlgorithm::processFilter()
{

}

PhaAlgorithm::PhaAlgorithm(const QVector<double> &dataOrigin, QObject *parent)
    : BaseAlgorithm{dataOrigin,parent}
{
}

QString PhaAlgorithm::getName() const
{
    return QString{"PHA"};
}
