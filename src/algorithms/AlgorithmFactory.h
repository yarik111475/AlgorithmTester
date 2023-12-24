#ifndef ALGORITHMFACTORY_H
#define ALGORITHMFACTORY_H
#include <QString>
#include <memory>

class BaseAlgorithm;
class AlgorithmFactory
{
public:
    explicit AlgorithmFactory()=delete;
    ~AlgorithmFactory()=default;
    AlgorithmFactory(const AlgorithmFactory& other)=delete;
    AlgorithmFactory& operator=(const AlgorithmFactory& other)=delete;
    static std::unique_ptr<BaseAlgorithm> makeAlgorithm(const QString& name,const QVector<double>& dataOrigin,double triggerThreshold,int span,int tau);
};

#endif // ALGORITHMFACTORY_H
