#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QLabel>
#include <QPushbutton>
#include <QMainWindow>
#include <QTimer>


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

private:
    Ui::MainWindow *ui;
    QLabel *time_label;
    QPushButton *start_button;
    QTimer *timer;

    int seconds_remaining;
    bool running;

private slots:
    void onTimerTick();
    void onStartPause();
};
#endif // MAINWINDOW_H
