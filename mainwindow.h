#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QLabel>
#include <QPushbutton>
#include <QMainWindow>
#include <QTimer>
#include <QWidget>

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow() override;

    enum Phase { Work, Short_Break, Long_Break };

private:
    Ui::MainWindow *ui;
    QLabel *time_label;
    QPushButton *start_button;
    QPushButton *reset_button;
    QTimer *timer;
    QWidget *side_bar;

    int seconds_remaining;
    bool running;

    static constexpr int TIME_CONSTANT=60;

    int work_duration;
    int short_break_duration;
    int long_break_duration;

    Phase current_phase;
    int work_phase_counter;

    void checkTimeOut();
    void setTimerLabel();
    int minsToSecs(int mins);

    void setupStart();
    void setupPause();

    QWidget* initUi(QPushButton* start, QLabel* timer_label, QPushButton* reset);

private slots:
    void onTimerTick();
    void onStartPause();
    void reset();
};
#endif // MAINWINDOW_H
