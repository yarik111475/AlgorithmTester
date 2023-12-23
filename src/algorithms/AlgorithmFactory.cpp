#include "AlgorithmFactory.h"
#include "BaseAlgorithm.h"
#include "PhaAlgorithm.h"

std::unique_ptr<BaseAlgorithm> AlgorithmFactory::makeAlgorithm(const QString &name, const QVector<double> &dataOrigin)
{
    std::unique_ptr<BaseAlgorithm> algorithmPtr {nullptr};
    algorithmPtr.reset(new PhaAlgorithm(dataOrigin));
    return algorithmPtr;
}
