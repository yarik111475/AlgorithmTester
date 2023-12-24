#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QVector>
#include <QString>
#include <memory>

class QwtPlot;
class QDial;
class QSlider;
class QComboBox;
class QLineEdit;
class QPushButton;
class BaseAlgorithm;
class MainWindow : public QMainWindow
{
    Q_OBJECT
private:
    QwtPlot* qwtPlotSmoothPtr_ {nullptr};
    QwtPlot* qwtPlotRCR2Ptr_ {nullptr};
    QwtPlot* qwtPlotFilteredPtr_ {nullptr};
    QLineEdit* tauLineEditPtr_ {nullptr};
    QLineEdit* spanLineEditPtr_ {nullptr};
    QLineEdit* thresholdLineEditPtr_ {nullptr};
    QSlider* leftSmoothOffsetSliderPtr_ {nullptr};
    QSlider* rightSmoothOffsetSliderPtr_ {nullptr};
    QSlider* leftRCCR2OffsetSliderPtr_ {nullptr};
    QSlider* rightRCCR2OffsetSliderPtr_ {nullptr};
    QSlider* leftFilteredOffsetSliderPtr_ {nullptr};
    QSlider* rightFilteredOffsetSliderPtr_ {nullptr};
    QComboBox* algoComboBoxPtr_ {nullptr};
    QPushButton* loadButtonPtr_ {nullptr};
    QPushButton* saveButtonPtr_ {nullptr};

    std::shared_ptr<BaseAlgorithm> algorithmPtr_ {nullptr};
    QVector<double> dataSmooth_ {};
    QVector<double> dataRCCR2_ {};
    QVector<double> dataFiltered_ {};

    QVector<double> readOrigin(const QString& path);
    void redrawPlot(QwtPlot * const plotPtr, const QVector<double> data, const QColor& color);
private slots:
    void loadSlot();
    void saveSlot();

    void endStepOneSlot(const QVector<double>& dataSmooth);
    void endStepTwoSlot(const QVector<double>& dataRCCR2);
    void endStepThreeSlot(const QVector<double>& dataFiltered);
    void finishedSlot(const QString& lastError);

    void leftSmoothChangedSlot(int value);
    void rightSmoothChangedSlot(int value);

    void leftRCCR2ChangedSlot(int value);
    void rightRCCR2ChangedSlot(int value);
public:
    explicit MainWindow(QWidget *parent = nullptr);

signals:

};

#endif // MAINWINDOW_H
