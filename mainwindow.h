#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QLabel>
#include <QPushbutton>
#include <QMainWindow>
#include <QTimer>
#include <QStackedWidget>
#include <QSpinBox>

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
    QPushButton *apply_settings;
    QTimer *timer;
    QPushButton *side_bar;

    //fields to change settings
    QWidget *settings_widget;
    QSpinBox *work_box;
    QSpinBox *short_break_box;
    QSpinBox *long_break_box;
    QSpinBox *num_sessions_box;



    int seconds_remaining;
    bool running;

    static constexpr int TIME_CONSTANT=60;

    int work_duration;
    int short_break_duration;
    int long_break_duration;

    Phase current_phase;
    int work_phase_counter;
    int max_work_phases;

    void checkTimeOut();
    void setTimerLabel();
    int minsToSecs(int mins);
    int secsToMins(int secs);

    void setupStart();
    void setupPause();

    QWidget* initUi(QPushButton* start, QLabel* timer_label, QPushButton* reset);

private slots:
    void onTimerTick();
    void onStartPause();
    void reset();
    void onSideTabClicked();
    void applySettings();
};
#endif // MAINWINDOW_H
