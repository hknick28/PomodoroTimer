#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QVBoxLayout>

//Function prototypes
QWidget* initUi(QPushButton* start, QLabel* timer_label);


MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    MainWindow::time_label = new QLabel("25:00");
    MainWindow::start_button = new QPushButton("Start");
    MainWindow::seconds_remaining = 25 * 60;
    MainWindow::running = false;
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
    int mins;
    int secs;

    seconds_remaining--;

    secs = seconds_remaining % 60;
    mins = seconds_remaining / 60;

    time_label->setText(QString::number(mins)+":"+QString::number(secs));
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
QWidget* initUi(QPushButton* start, QLabel* timer_label)
{
    QWidget *central_widget = new QWidget();
    QVBoxLayout *layout = new QVBoxLayout(central_widget);

    layout->setAlignment(Qt::AlignCenter);

    layout->addWidget(timer_label);
    layout->addWidget(start);

    return central_widget;
}