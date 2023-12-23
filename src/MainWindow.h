#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

class QwtPlot;
class MainWindow : public QMainWindow
{
    Q_OBJECT
private:
    QwtPlot* qwtPlotPtr_ {};
public:
    explicit MainWindow(QWidget *parent = nullptr);

signals:

};

#endif // MAINWINDOW_H
