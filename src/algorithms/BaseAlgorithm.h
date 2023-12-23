#ifndef BASEALGORITHM_H
#define BASEALGORITHM_H

#include <QThread>
#include <QVector>
#include <QString>

class BaseAlgorithm : public QThread
{
    Q_OBJECT
protected:
    QVector<double> dataOrigin_ {};
    virtual void run()override;
    virtual void processSmooth()=0;
    virtual void processRCCR2()=0;
    virtual void processFilter()=0;
public:
    explicit BaseAlgorithm(const QVector<double>& dataOrigin,QObject* parent=nullptr);
    virtual ~BaseAlgorithm();
    virtual QString getName()const=0;
signals:
    void endSmoothSignal(const QVector<double>& dataSmooth);
    void endRCCR2Signal(const QVector<double>& dataRCCR2);
    void endFilterSignal(const QVector<double>& dataFiltered);
    void finishedSignal(const QString& lastError);

};

#endif // BASEALGORITHM_H
