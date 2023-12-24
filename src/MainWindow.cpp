#include "MainWindow.h"
#include "qwt_plot.h"
#include "qwt_plot_grid.h"
#include "qwt_plot_panner.h"
#include "qwt_plot_magnifier.h"
#include "qwt_plot_curve.h"
#include "algorithms/PhaAlgorithm.h"
#include "algorithms/BaseAlgorithm.h"
#include "algorithms/AlgorithmFactory.h"

#include <QFrame>
#include <QFile>
#include <QDial>
#include <QLabel>
#include <QSlider>
#include <QComboBox>
#include <QLineEdit>
#include <QPushButton>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QGridLayout>
#include <QGroupBox>
#include <QTextStream>
#include <QMessageBox>
#include <QFileDialog>
#include <QStringList>
#include <QApplication>
#include <QIntValidator>
#include <QDoubleValidator>
#include <algorithm>

QVector<double> MainWindow::readOrigin(const QString &path)
{
    QFile file{path};
    QVector<double> dataOrigin {};
    if(file.open(QIODevice::ReadOnly)){
        QTextStream stream {&file};
        while(!stream.atEnd()){
            const QString line {stream.readLine()};
            const double value {line.section('\t',1,1).toDouble()};
            dataOrigin.push_back(value);
        }
        file.close();
    }
    return dataOrigin;
}

void MainWindow::redrawPlot(QwtPlot * const plotPtr, const QVector<double> data, const QColor &color)
{
    if(!data.empty()){
        QPolygonF points{};
        points.reserve(data.size());
        double index {0.0};
        std::for_each(data.begin(),data.end(),[&](double value){
            points.push_back(QPointF(index,value));
            ++index;
        });
        QwtPlotCurve* qwtCurvePtr {new QwtPlotCurve{}};
        qwtCurvePtr->setRenderHint(QwtPlotItem::RenderAntialiased,true);
        qwtCurvePtr->setPen(color,2);
        qwtCurvePtr->setSamples(points);

        plotPtr->detachItems();
        qwtCurvePtr->attach(plotPtr);
        plotPtr->setAxisScale(QwtPlot::xBottom,0,data.size());
        plotPtr->replot();
    }
}

void MainWindow::loadSlot()
{
    const QString filter {"Comma-separated files (*.csv)"};
    const QString filename {QFileDialog::getOpenFileName(this,QObject::tr("Open file"),qApp->applicationDirPath(),filter)};
    if(!filename.isEmpty()){
        const int span {spanLineEditPtr_->text().toInt()};
        int tau {tauLineEditPtr_->text().toInt()};
        const double triggerThreshold {thresholdLineEditPtr_->text().toDouble()};

        const auto dataOrigin {readOrigin(filename)};
        if(!dataOrigin.isEmpty()){
            const QString algorithmName {algoComboBoxPtr_->currentText()};
            if(!algorithmName.isEmpty() && !dataOrigin.empty()){
                algorithmPtr_=AlgorithmFactory::makeAlgorithm(algorithmName,dataOrigin,triggerThreshold,span,tau);
                if(algorithmPtr_){
                    QObject::connect(algorithmPtr_.get(),&BaseAlgorithm::endStepOneSignal,
                                     this,&MainWindow::endStepOneSlot);
                    QObject::connect(algorithmPtr_.get(),&BaseAlgorithm::endStepTwoSignal,
                                     this,&MainWindow::endStepTwoSlot);
                    QObject::connect(algorithmPtr_.get(),&BaseAlgorithm::endStepThreeSignal,
                                     this,&MainWindow::endStepThreeSlot);
                    QObject::connect(algorithmPtr_.get(),&BaseAlgorithm::finishedSignal,
                                     this,&MainWindow::finishedSlot);
                    QObject::connect(algorithmPtr_.get(),&QThread::finished,
                                     algorithmPtr_.get(),&QThread::deleteLater);
                    algorithmPtr_->start();
                }
            }
        }
    }
}

void MainWindow::saveSlot()
{

}

void MainWindow::endStepOneSlot(const QVector<double> &dataSmooth)
{
    if(!dataSmooth.empty()){
        dataSmooth_=dataSmooth;
        redrawPlot(qwtPlotSmoothPtr_,dataSmooth_,Qt::red);
        leftSmoothOffsetSliderPtr_->setRange(0,dataSmooth_.size());
        leftSmoothOffsetSliderPtr_->setValue(0);
        rightSmoothOffsetSliderPtr_->setRange(0,dataSmooth_.size());
        rightSmoothOffsetSliderPtr_->setValue(0);
    }
}

void MainWindow::endStepTwoSlot(const QVector<double> &dataRCCR2)
{
    if(!dataRCCR2.empty()){
        dataRCCR2_=dataRCCR2;
        redrawPlot(qwtPlotRCR2Ptr_,dataRCCR2_,Qt::blue);
    }
}

void MainWindow::endStepThreeSlot(const QVector<double> &dataFiltered)
{
    if(!dataFiltered.empty()){
        dataFiltered_=dataFiltered;
        redrawPlot(qwtPlotFilteredPtr_,dataFiltered_,Qt::green);
    }
}

void MainWindow::finishedSlot(const QString &lastError)
{
    qDebug()<<lastError;
}

void MainWindow::leftSmoothChangedSlot(int value)
{
    if(!dataSmooth_.empty() && value!=0 && value!=dataSmooth_.size()){
        const QVector<double> data {dataSmooth_.mid(value,rightSmoothOffsetSliderPtr_->value()-value)};
        redrawPlot(qwtPlotSmoothPtr_,data,Qt::red);
    }
}

void MainWindow::rightSmoothChangedSlot(int value)
{
    if(!dataSmooth_.empty() && value!=0 && value!=dataSmooth_.size()){
        const QVector<double> data {dataSmooth_.mid(leftSmoothOffsetSliderPtr_->value(),dataSmooth_.size()-value)};
        redrawPlot(qwtPlotSmoothPtr_,data,Qt::red);
    }
}

void MainWindow::leftRCCR2ChangedSlot(int value)
{

}

void MainWindow::rightRCCR2ChangedSlot(int value)
{

}

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent),
      qwtPlotSmoothPtr_{new QwtPlot(this)},
      qwtPlotRCR2Ptr_{new QwtPlot(this)},
      qwtPlotFilteredPtr_{new QwtPlot(this)},
      tauLineEditPtr_{new QLineEdit("500")},
      spanLineEditPtr_{new QLineEdit("5")},
      thresholdLineEditPtr_{new QLineEdit("3450")},
      leftSmoothOffsetSliderPtr_{new QSlider(Qt::Horizontal)},
      rightSmoothOffsetSliderPtr_{new QSlider(Qt::Horizontal)},
      leftRCCR2OffsetSliderPtr_{new QSlider(Qt::Horizontal)},
      rightRCCR2OffsetSliderPtr_{new QSlider(Qt::Horizontal)},
      leftFilteredOffsetSliderPtr_{new QSlider(Qt::Horizontal)},
      rightFilteredOffsetSliderPtr_{new QSlider(Qt::Horizontal)},
      algoComboBoxPtr_{new QComboBox},
      loadButtonPtr_{new QPushButton(QObject::tr("Load"))},
      saveButtonPtr_{new QPushButton(QObject::tr("Save"))}
{


    QGroupBox* algoGroupBoxPtr {new QGroupBox(QObject::tr("Algorithm params"))};
    QGridLayout* algoGridLayoutPtr {new QGridLayout};
    algoGridLayoutPtr->addWidget(new QLabel(QObject::tr("Algorithm:")),0,0);
    algoGridLayoutPtr->addWidget(algoComboBoxPtr_,0,1);

    algoGridLayoutPtr->addWidget(new QLabel(QObject::tr("Tau:")),1,0);
    algoGridLayoutPtr->addWidget(tauLineEditPtr_,1,1);
    tauLineEditPtr_->setValidator(new QIntValidator(tauLineEditPtr_));

    algoGridLayoutPtr->addWidget(new QLabel(QObject::tr("Span:")),2,0);
    algoGridLayoutPtr->addWidget(spanLineEditPtr_,2,1);
    spanLineEditPtr_->setValidator(new QIntValidator(spanLineEditPtr_));

    algoGridLayoutPtr->addWidget(new QLabel(QObject::tr("Threshold:")),3,0);
    algoGridLayoutPtr->addWidget(thresholdLineEditPtr_,3,1);
    thresholdLineEditPtr_->setValidator(new QIntValidator(thresholdLineEditPtr_));

    algoGridLayoutPtr->addWidget(loadButtonPtr_,4,1);
    algoGridLayoutPtr->addWidget(saveButtonPtr_,5,1);

    algoGroupBoxPtr->setLayout(algoGridLayoutPtr);

    QGroupBox* offsetGroupBoxPtr {new QGroupBox(QObject::tr("Offset params"))};
    QGridLayout* offsetGridLayoutPtr {new QGridLayout};

    QFrame* lineAPtr {new QFrame};
    lineAPtr->setSizePolicy(QSizePolicy::Expanding,QSizePolicy::Fixed);
    lineAPtr->setFrameShadow(QFrame::Sunken);
    lineAPtr->setFrameShape(QFrame::HLine);
    lineAPtr->setFixedHeight(2);

    QFrame* lineBPtr {new QFrame};
    lineBPtr->setSizePolicy(QSizePolicy::Expanding,QSizePolicy::Fixed);
    lineBPtr->setFrameShadow(QFrame::Sunken);
    lineBPtr->setFrameShape(QFrame::HLine);
    lineBPtr->setFixedHeight(2);

    offsetGridLayoutPtr->addWidget(new QLabel(QObject::tr("Left smooth offset:")),0,0);
    offsetGridLayoutPtr->addWidget(leftSmoothOffsetSliderPtr_,0,1);
    leftSmoothOffsetSliderPtr_->setSingleStep(1000);
    QObject::connect(leftSmoothOffsetSliderPtr_,&QSlider::valueChanged,
                     this,&MainWindow::leftSmoothChangedSlot);

    offsetGridLayoutPtr->addWidget(new QLabel(QObject::tr("Right smooth offset:")),1,0);
    offsetGridLayoutPtr->addWidget(rightSmoothOffsetSliderPtr_,1,1);
    rightSmoothOffsetSliderPtr_->setSingleStep(1000);
    QObject::connect(rightSmoothOffsetSliderPtr_,&QSlider::valueChanged,
                     this,&MainWindow::rightSmoothChangedSlot);

    offsetGridLayoutPtr->addWidget(lineAPtr,2,0,1,2);
    offsetGridLayoutPtr->addWidget(new QLabel(QObject::tr("Left RCCR2 offset:")),4,0);
    offsetGridLayoutPtr->addWidget(leftRCCR2OffsetSliderPtr_,4,1);
    offsetGridLayoutPtr->addWidget(new QLabel(QObject::tr("Right RCCR2 offset:")),5,0);
    offsetGridLayoutPtr->addWidget(rightRCCR2OffsetSliderPtr_,5,1);
    offsetGridLayoutPtr->addWidget(lineBPtr,6,0,1,2);
    offsetGridLayoutPtr->addWidget(new QLabel(QObject::tr("Left filtered offset:")),7,0);
    offsetGridLayoutPtr->addWidget(leftFilteredOffsetSliderPtr_,7,1);
    offsetGridLayoutPtr->addWidget(new QLabel(QObject::tr("Right filtered offset:")),8,0);
    offsetGridLayoutPtr->addWidget(rightFilteredOffsetSliderPtr_,8,1);

    offsetGroupBoxPtr->setLayout(offsetGridLayoutPtr);

    QVBoxLayout* rightVBoxLayoutPtr {new QVBoxLayout};
    rightVBoxLayoutPtr->addWidget(algoGroupBoxPtr);
    rightVBoxLayoutPtr->addWidget(offsetGroupBoxPtr);
    rightVBoxLayoutPtr->addStretch(5);

    QVBoxLayout* leftVBoxLayoutPtr {new QVBoxLayout};
    leftVBoxLayoutPtr->addWidget(qwtPlotSmoothPtr_);
    leftVBoxLayoutPtr->addWidget(qwtPlotRCR2Ptr_);
    leftVBoxLayoutPtr->addWidget(qwtPlotFilteredPtr_);

    QHBoxLayout* mainHBoxLayoutPtr {new QHBoxLayout};
    mainHBoxLayoutPtr->addLayout(leftVBoxLayoutPtr,5);
    mainHBoxLayoutPtr->addLayout(rightVBoxLayoutPtr,1);

    QWidget* centralWidgetPtr {new QWidget};
    centralWidgetPtr->setLayout(mainHBoxLayoutPtr);
    setCentralWidget(centralWidgetPtr);

    QObject::connect(loadButtonPtr_,&QPushButton::clicked,
                     this,&MainWindow::loadSlot);
    QObject::connect(saveButtonPtr_,&QPushButton::clicked,
                     this,&MainWindow::saveSlot);

    algoComboBoxPtr_->addItems({"PHA"});

    qwtPlotSmoothPtr_->setTitle(QObject::tr("Smoothed data"));
    qwtPlotSmoothPtr_->setCanvasBackground(Qt::white);

    qwtPlotRCR2Ptr_->setTitle(QObject::tr("RCCR2 data"));
    qwtPlotRCR2Ptr_->setCanvasBackground(Qt::white);

    qwtPlotFilteredPtr_->setTitle(QObject::tr("Filtered data"));
    qwtPlotFilteredPtr_->setCanvasBackground(Qt::white);
}
