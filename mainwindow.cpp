#include "mainwindow.h"
//#include "ui_mainwindow.h"
#include <QVBoxLayout>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    /*, ui(new Ui::MainWindow)*/
{
   // ui->setupUi(this);

    MainWindow::time_label = new QLabel("25:00");
    MainWindow::start_button = new QPushButton("Start");
    MainWindow::reset_button = new QPushButton("Reset");
    MainWindow::side_bar = new QPushButton("☰");
    MainWindow::apply_settings = new QPushButton("Apply");

    MainWindow::seconds_remaining = 25 * TIME_CONSTANT;
    MainWindow::running = false;
    MainWindow::current_phase = Phase::Work;
    MainWindow::work_phase_counter = 0;
    MainWindow::max_work_phases = 4;

    MainWindow::work_duration=minsToSecs(25);
    MainWindow::short_break_duration=minsToSecs(5);
    MainWindow::long_break_duration=minsToSecs(10);


    MainWindow::settings_widget = new QWidget();
    MainWindow::work_box = new QSpinBox(this);
    MainWindow::short_break_box = new QSpinBox(this);
    MainWindow::long_break_box = new QSpinBox(this);
    MainWindow::num_sessions_box = new QSpinBox(this);

    MainWindow::timer = new QTimer(this);

    work_box->setValue(secsToMins(work_duration));
    short_break_box->setValue(secsToMins(short_break_duration));
    long_break_box->setValue(secsToMins(long_break_duration));
    num_sessions_box->setValue(max_work_phases);

    time_label->setAlignment(Qt::AlignHCenter);
    settings_widget->hide();

    connect(timer, &QTimer::timeout, this, &MainWindow::onTimerTick);
    connect(start_button, &QPushButton::pressed, this, &MainWindow::onStartPause);
    connect(reset_button, &QPushButton::pressed, this, &MainWindow::reset);
    connect(side_bar, &QPushButton::pressed, this, &MainWindow::onSideTabClicked);
    connect(apply_settings, &QPushButton::pressed, this, &MainWindow::applySettings);

    this -> setCentralWidget(initUi(start_button, time_label, reset_button));
}

MainWindow::~MainWindow(){}

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
    QWidget *top_panel = new QWidget();
    QWidget *central_widget = new QWidget();
    QWidget *content_area = new QWidget();

    QWidget *root_widget = new QWidget();

    QVBoxLayout *root_layout = new QVBoxLayout(root_widget);
    QVBoxLayout *timer_layout = new QVBoxLayout(central_widget);
    QHBoxLayout *top_layout = new QHBoxLayout(top_panel);
    QVBoxLayout *settings_layout = new QVBoxLayout(settings_widget);
    QHBoxLayout *content_area_layout = new QHBoxLayout(content_area);

    root_layout->setSpacing(0);
    top_layout->setSpacing(0);
    top_panel->setFixedHeight(50);
    central_widget->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);


    top_layout->setAlignment(Qt::AlignLeft);
    top_layout->addWidget(side_bar);
    top_layout->setContentsMargins(0, 0, 0, 0);


    timer_layout->setAlignment(Qt::AlignCenter);
    timer_layout->addWidget(timer_label);
    timer_layout->addWidget(start);
    timer_layout->addWidget(reset);

    settings_layout->addWidget(new QLabel("Work Duration (mins)"));
    settings_layout->addWidget(work_box);
    settings_layout->addWidget(new QLabel("Short Break Duration (mins)"));
    settings_layout->addWidget(short_break_box);
    settings_layout->addWidget(new QLabel("Long Break Duration (mins)"));
    settings_layout->addWidget(long_break_box);
    settings_layout->addWidget(new QLabel("Work Sessions Before Long Break (mins)"));
    settings_layout->addWidget(num_sessions_box);
    settings_layout->addWidget(apply_settings);
    settings_layout->setAlignment(Qt::AlignTop);

    content_area_layout->addWidget(settings_widget);
    content_area_layout->addWidget(central_widget);

    root_layout->addWidget(top_panel);
    root_layout->addWidget(content_area);
    root_layout->setContentsMargins(0, 0, 0, 0);

    return root_widget;
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
        if(work_phase_counter == max_work_phases)
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
    if(mins >= 10){ mins_str = QString::number(mins); }
    else{ mins_str = + "0" + QString::number(mins); }


    if(secs >= 10){ secs_str = QString::number(secs); }
    else{ secs_str = "0"+QString::number(secs); }


    time_label->setText(mins_str+":"+secs_str);
}

int MainWindow::minsToSecs(int mins)
{
    return mins*TIME_CONSTANT;
}

int MainWindow::secsToMins(int secs)
{
    return secs/60;
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

/*
 * Open side tab when 3-lines button clicked
 * Expose the containers for editing timer settings
 * */
void MainWindow::onSideTabClicked()
{
    if(settings_widget->isVisible()){ settings_widget->hide(); }
    else{ settings_widget->show(); }
}

/*
 * Reset timer varaibles to match user selected inputs from boxes
 * */
void MainWindow::applySettings()
{
    settings_widget->close();
    work_duration = minsToSecs(work_box->value());
    short_break_duration = minsToSecs(short_break_box->value());
    long_break_duration = minsToSecs(long_break_box->value());
    max_work_phases = num_sessions_box->value();
    reset();
}