#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QVector>
#include <QString>
#include <memory>

class QwtPlot;
class QDial;
class QComboBox;
class QPushButton;
class BaseAlgorithm;
class MainWindow : public QMainWindow
{
    Q_OBJECT
private:
    QwtPlot* qwtPlotPtr_ {nullptr};
    QDial* xAxisDialPtr_ {nullptr};
    QDial* yAxisDialPtr_ {nullptr};
    QComboBox* algoComboBoxPtr_ {nullptr};
    QPushButton* openButtonPtr_ {nullptr};
    QPushButton* processButtonPtr_ {nullptr};
    std::shared_ptr<BaseAlgorithm> algorithmPtr_ {nullptr};
    QVector<double> readOrigin(const QString& path);
private slots:
    void openSlot();
    void processSlot();
public:
    explicit MainWindow(QWidget *parent = nullptr);

signals:

};

#endif // MAINWINDOW_H
