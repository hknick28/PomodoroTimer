#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QVBoxLayout>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    MainWindow::time_label = new QLabel("25:00");
    MainWindow::start_button = new QPushButton("Start");
    MainWindow::seconds_remaining = 25 * TIME_CONSTANT;
    MainWindow::running = false;
    MainWindow::current_phase = Phase::Work;
    MainWindow::work_phase_counter = 0;
    MainWindow::work_duration=minsToSecs(25);
    MainWindow::short_break_duration=minsToSecs(5);
    MainWindow::long_break_duration=minsToSecs(10);
    MainWindow::timer = new QTimer(this);

    time_label->setAlignment(Qt::AlignHCenter);

    connect(timer, &QTimer::timeout, this, &MainWindow::onTimerTick);
    connect(start_button, &QPushButton::pressed, this, &MainWindow::onStartPause);

    this -> setCentralWidget(initUi(start_button, time_label));
}

MainWindow::~MainWindow()
{
    delete ui;
}

/*
 * Every second, reduces the time by one second
 * Updates timer label
 * */
void MainWindow::onTimerTick()
{
    seconds_remaining--;
    setTimerLabel();
    checkTimeOut();
}

/*
 * Triggers when start/pause button is pressed
 * If running then pauses the timer
 * If not running then starts the timer
 *
 * Changes the button label accordingly
 * */
void MainWindow::onStartPause()
{
    if(running)
    {
        running=false;
        start_button->setText("Start");
        timer->stop();
    }
    else
    {
        running=true;
        start_button->setText("Pause");
        timer->start(1000);
    }
}

/*
 * Create Layout for the gui
 * Adding the start button
 * */
QWidget* MainWindow::initUi(QPushButton* start, QLabel* timer_label)
{
    QWidget *central_widget = new QWidget();
    QVBoxLayout *layout = new QVBoxLayout(central_widget);

    layout->setAlignment(Qt::AlignCenter);

    layout->addWidget(timer_label);
    layout->addWidget(start);

    return central_widget;
}

/*
 * Check if there is not remaining time
 * Based on the phase, reset the timer to the next phase
 * After every x work phases, trigger long break
 * */
void MainWindow::checkTimeOut()
{
    if(seconds_remaining > 0){ return; }

    switch(current_phase)
    {
    case Phase::Work:
        work_phase_counter++;
        if(work_phase_counter == 4)
        {
            seconds_remaining = long_break_duration;
            setTimerLabel();
            work_phase_counter=0;
            current_phase = Phase::Long_Break;
        }
        else
        {
            seconds_remaining = short_break_duration;
            setTimerLabel();
            current_phase = Phase::Short_Break;
        }

        break;

    case Phase::Short_Break:
        seconds_remaining = work_duration;
        setTimerLabel();
        current_phase = Phase::Work;
        break;

    case Phase::Long_Break:
        seconds_remaining = work_duration;
        setTimerLabel();
        current_phase = Phase::Work;
        break;

    }
}

void MainWindow::setTimerLabel()
{
    int secs;
    int mins;

    secs = seconds_remaining % TIME_CONSTANT;
    mins = seconds_remaining / TIME_CONSTANT;

    time_label->setText(QString::number(mins)+":"+QString::number(secs));
}

int MainWindow::minsToSecs(int mins)
{
    return mins*TIME_CONSTANT;
}
