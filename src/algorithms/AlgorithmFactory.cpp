#include "AlgorithmFactory.h"
#include "BaseAlgorithm.h"
#include "PhaAlgorithm.h"

std::unique_ptr<BaseAlgorithm> AlgorithmFactory::makeAlgorithm(const QString &name, const QVector<double> &dataOrigin, double triggerThreshold, int span, int tau)
{
    std::unique_ptr<BaseAlgorithm> algorithmPtr {nullptr};
    algorithmPtr.reset(new PhaAlgorithm(dataOrigin,triggerThreshold,span,tau));
    return algorithmPtr;
}
