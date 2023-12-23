#include "MainWindow.h"
#include "qwt_plot.h"
#include "algorithms/PhaAlgorithm.h"
#include "algorithms/BaseAlgorithm.h"
#include "algorithms/AlgorithmFactory.h"

#include <QDial>
#include <QComboBox>
#include <QPushButton>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QGridLayout>
#include <QGroupBox>

void MainWindow::openSlot()
{

}

void MainWindow::processSlot()
{

}

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent),
      qwtPlotPtr_{new QwtPlot(this)},
      xAxisDialPtr_{new QDial},
      yAxisDialPtr_{new QDial},
      algoComboBoxPtr_{new QComboBox},
      openButtonPtr_{new QPushButton(QObject::tr("Open"))},
      processButtonPtr_{new QPushButton(QObject::tr("Process"))}
{
    QGroupBox* algoGroupBoxPtr {new QGroupBox(QObject::tr("Algorithms"))};
    QVBoxLayout* algoVBoxLayoutPtr {new QVBoxLayout};
    algoVBoxLayoutPtr->addWidget(algoComboBoxPtr_);
    algoVBoxLayoutPtr->addWidget(openButtonPtr_);
    algoVBoxLayoutPtr->addWidget(processButtonPtr_);
    algoVBoxLayoutPtr->addStretch(5);
    algoGroupBoxPtr->setLayout(algoVBoxLayoutPtr);

    QGroupBox* axisGroupBoxPtr {new QGroupBox(QObject::tr("Axis"))};
    QVBoxLayout* axisVBoxLayoutPtr {new QVBoxLayout};
    axisVBoxLayoutPtr->addWidget(xAxisDialPtr_);
    axisVBoxLayoutPtr->addWidget(yAxisDialPtr_);
    axisVBoxLayoutPtr->addStretch(5);
    axisGroupBoxPtr->setLayout(axisVBoxLayoutPtr);

    QVBoxLayout* rightVBoxLayoutPtr {new QVBoxLayout};
    rightVBoxLayoutPtr->addWidget(algoGroupBoxPtr);
    rightVBoxLayoutPtr->addWidget(axisGroupBoxPtr);
    rightVBoxLayoutPtr->addStretch(5);

    QVBoxLayout* leftVBoxLayoutPtr {new QVBoxLayout};
    leftVBoxLayoutPtr->addWidget(qwtPlotPtr_);

    QHBoxLayout* mainHBoxLayoutPtr {new QHBoxLayout};
    mainHBoxLayoutPtr->addLayout(leftVBoxLayoutPtr,5);
    mainHBoxLayoutPtr->addLayout(rightVBoxLayoutPtr,0);

    QWidget* centralWidgetPtr {new QWidget};
    centralWidgetPtr->setLayout(mainHBoxLayoutPtr);
    setCentralWidget(centralWidgetPtr);

    QVector<double> dataOrigin {};
    const QString algorithmName {};
    algorithmPtr_=AlgorithmFactory::makeAlgorithm(algorithmName,dataOrigin);
}
