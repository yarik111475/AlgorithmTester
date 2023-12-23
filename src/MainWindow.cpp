#include "MainWindow.h"
#include "qwt_plot.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent),qwtPlotPtr_{new QwtPlot(this)}
{
    setCentralWidget(qwtPlotPtr_);
}
