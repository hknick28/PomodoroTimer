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
    MainWindow::reset_button = new QPushButton("Reset");
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
    connect(reset_button, &QPushButton::pressed, this, &MainWindow::reset);


    this -> setCentralWidget(initUi(start_button, time_label, reset_button));
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
    if(running){ setupStart(); }
    else{ setupPause(); }
}

void MainWindow::setupStart()
{
    running=false;
    start_button->setText("Start");
    timer->stop();
}

void MainWindow::setupPause()
{
    running=true;
    start_button->setText("Pause");
    timer->start(1000);
}

/*
 * Create Layout for the gui
 * Adding the start button
 * */
QWidget* MainWindow::initUi(QPushButton* start, QLabel* timer_label, QPushButton* reset)
{
    QWidget *central_widget = new QWidget();
    QVBoxLayout *layout = new QVBoxLayout(central_widget);

    layout->setAlignment(Qt::AlignCenter);

    layout->addWidget(timer_label);
    layout->addWidget(start);
    layout->addWidget(reset);

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

    QString mins_str;
    QString secs_str;


    secs = seconds_remaining % TIME_CONSTANT;
    mins = seconds_remaining / TIME_CONSTANT;

    mins_str = QString::number(mins);

    //Add padding if neccecary
    if(mins < 10){ mins_str = QString::number(mins); }
    else{ mins_str = + "0" + QString::number(mins); }


    if(secs < 10){ secs_str = QString::number(secs); }
    else{ secs_str = QString::number(secs) + "0"; }


    time_label->setText(mins_str+":"+secs_str);
}

int MainWindow::minsToSecs(int mins)
{
    return mins*TIME_CONSTANT;
}

/*
 * Reset timer back to working phase
 * */
void MainWindow::reset()
{
    current_phase = Phase::Work;
    seconds_remaining = work_duration;
    work_phase_counter = 0;
    setupStart();
    setTimerLabel();
}
