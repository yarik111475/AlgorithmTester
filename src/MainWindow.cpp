#include "MainWindow.h"
#include "qwt_plot.h"
#include "algorithms/PhaAlgorithm.h"
#include "algorithms/BaseAlgorithm.h"
#include "algorithms/AlgorithmFactory.h"

#include <QFile>
#include <QDial>
#include <QComboBox>
#include <QPushButton>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QGridLayout>
#include <QGroupBox>
#include <QTextStream>
#include <QMessageBox>
#include <QStringList>

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

    QObject::connect(openButtonPtr_,&QPushButton::clicked,
                     this,&MainWindow::openSlot);
    QObject::connect(processButtonPtr_,&QPushButton::clicked,
                     this,&MainWindow::processSlot);
    algoComboBoxPtr_->addItems({"PHA"});

    QVector<double> dataOrigin {};
    const QString algorithmName {};
    algorithmPtr_=AlgorithmFactory::makeAlgorithm(algorithmName,dataOrigin);
}
