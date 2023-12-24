#include "PhaAlgorithm.h"
#include <algorithm>
#include <cmath>

QPair<int, int> PhaAlgorithm::makeFilterParams()
{
    const auto dataSmooth {makeSmooth(dataOrigin_,span_)};
    const int samples {dataSmooth.size()};
    int triggerThresholdIdx {};
    for(int i=0;i<samples-1;++i){
        if(dataSmooth[i] >= triggerThreshold_ && dataSmooth[i+1] > triggerThreshold_){
            triggerThresholdIdx=i;
            break;
        }
    }

    const auto dataRCCR2 {makeRCCR2(dataOrigin_)};
    int shapedFlatTopIdx {};
    int shapedZeroCrossIdx {};
    for(int i=triggerThresholdIdx;i < samples+1-triggerThresholdIdx;++i){
        if(dataRCCR2[i]<dataRCCR2[i+1]){
            continue;
        }
        shapedFlatTopIdx=i;
        for(int j=shapedFlatTopIdx;j<samples+1-shapedFlatTopIdx;++j,++i){
            if(std::signbit(dataRCCR2[j])!=std::signbit(dataRCCR2[j+1])){
                shapedZeroCrossIdx=j;
                break;
            }
        }
        break;
    }
    const int k {std::abs(shapedFlatTopIdx-triggerThresholdIdx)};
    const int l {std::abs(shapedZeroCrossIdx-shapedFlatTopIdx) * 2};
    return QPair<int,int>{k,l};
}

QVector<double> PhaAlgorithm::makeSmooth(const QVector<double> &data, int span)
{
    const int shift   {span_/2};
    const int samples {dataOrigin_.size()};
    QVector<double> dataSmooth(samples);

    for(auto i {0};i<samples;++i){
        if((i==0) | (i==samples-1)){
            dataSmooth[i]=dataOrigin_[i];
        }
        else if((i<shift) | ((dataOrigin_.size()-i)<=shift)){
            if(i<shift){
                auto sum {0.0};
                auto distance {0};
                for(auto j{i-i};j<=i+i;++j){
                    sum+=dataOrigin_[j];
                    ++distance;
                }
                dataSmooth[i]=sum/distance;
            }
            else{
                auto sum {0.0};
                auto distance {0};
                for(auto j{i-(dataOrigin_.size()-i-1)};j<=i+(dataOrigin_.size()-i-1);++j){
                    sum+=dataOrigin_[j];
                    ++distance;
                }
                dataSmooth[i]=sum/distance;
            }
        }
        else{
            auto sum {0.0};
            for(auto j {i-shift};j<=i+shift;++j){
                sum+=dataOrigin_[j];
            }
            dataSmooth[i]=sum/span_;
        }
    }
    return dataSmooth;
}

QVector<double> PhaAlgorithm::makeRCCR2(const QVector<double> &data)
{
    const int samples {dataOrigin_.size()};
    const auto average {std::accumulate(dataOrigin_.begin(),dataOrigin_.end(),0)/samples};
    QVector<double> Y1(samples);
    QVector<double> Y2(samples);
    Y1[0]=average;
    Y2[0]=average;
    QVector<double> dataRCCR2(samples);
    for (int i = 1; i < samples; i++) {
        Y1[i] = 0.15 * dataOrigin_[i] + 0.84 * Y1[i - 1];
        Y2[i] = 0.91 * dataOrigin_[i] - 0.91 * Y1[i - 1] + 0.84 * Y2[i - 1];
        dataRCCR2[i] = 0.91 * Y2[i] - 0.91 * Y2[i - 1] + 0.84 * dataRCCR2[i - 1];
    }
    return dataRCCR2;
}

void PhaAlgorithm::processStepOne()
{
    const auto dataSmooth {makeSmooth(dataOrigin_,span_)};
    emit endStepOneSignal(dataSmooth);
}

void PhaAlgorithm::processStepTwo()
{
    const auto dataRCCR2 {makeRCCR2(dataOrigin_)};
    emit endStepTwoSignal(dataRCCR2);
}

void PhaAlgorithm::processStepThree()
{
    const auto filterParams {makeFilterParams()};
    const auto dataSmooth {makeSmooth(dataOrigin_,span_)};
    const int samples {dataSmooth.size()};
    const int k {filterParams.first};
    const int l {filterParams.second};

    QVector<double> a_k_l (samples);
    QVector<double> b (samples);
    QVector<double> c (samples);
    QVector<double> dataFiltered(samples);

    for(int i=1+k+l;i < samples-k-l; ++i){
        a_k_l[i]=dataSmooth[i]-dataSmooth[i-k]-dataSmooth[i-l]+dataSmooth[i-k-l];
        b[i]=b[i-1]+a_k_l[i];
        c[i]=b[i]+a_k_l[i] * tau_;
        dataFiltered[i]=dataFiltered[i-1]+c[i];
    }
    emit endStepThreeSignal(dataFiltered);
}

PhaAlgorithm::PhaAlgorithm(const QVector<double> &dataOrigin, double triggerThreshold, int span, int tau, QObject *parent)
    : BaseAlgorithm{dataOrigin,triggerThreshold,span,tau,parent}
{
}

QString PhaAlgorithm::getName() const
{
    return QString{"PHA"};
}
