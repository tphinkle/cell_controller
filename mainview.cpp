#include "mainview.h"
#include "ui_mainview.h"
#include <iostream>

MainView::MainView(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainView)

{

    ui->setupUi(this);

    setup();



}

void MainView::setup()
{
    setup_main_view();
    setup_rp_view();
    setup_oi_view();
    setup_syringe_view();

    return;
}

void MainView::setup_main_view()
{
    this->setWindowState(Qt::WindowMaximized);
    this->setWindowTitle("Cell controller");



    return;
}

void MainView::setup_rp_view()
{
    // Plot and plot items
    rp_plot_ = new QwtPlot(QwtText("RP Plot"), this);
    rp_plot_->setGeometry(0, 500, 480, 480);

    rp_plot_curve_ = new QwtPlotCurve(QwtText("RP Data"));
    rp_plot_curve_->attach(rp_plot_);

    // Buttons
    rp_create_task_button_ = new QPushButton("Create RP\n task", this);
    rp_create_task_button_->setGeometry(0, 100, 50, 25);

    rp_start_task_button_ = new QPushButton("Start RP\n task", this);
    rp_start_task_button_->setGeometry(50, 100, 50, 25);






    return;
}

void MainView::setup_oi_view()
{
    return;
}

void MainView::setup_syringe_view()
{

    // Buttons
    syringe_local_button_ = new QPushButton("Local", this);
    syringe_local_button_->setGeometry(0, 25, 50, 25);

    syringe_remote_button_ = new QPushButton("Remote", this);
    syringe_remote_button_->setGeometry(0, 25, 50, 25);

    syringe_forward_button_ = new QPushButton("Forward", this);
    syringe_forward_button_->setGeometry(0, 0, 50, 25);


    syringe_stop_button_ = new QPushButton("Stop", this);
    syringe_stop_button_->setGeometry(50, 0, 50, 25);


    syringe_reverse_button_ = new QPushButton("Reverse", this);
    syringe_reverse_button_->setGeometry(100, 0, 50, 25);


    syringe_switch_button_ = new QPushButton("Switch", this);
    syringe_switch_button_->setGeometry(150, 0, 50, 25);

    syringe_set_rate_button_ =  new QPushButton("Set", this);
    syringe_set_rate_button_->setGeometry(100, 50, 50, 25);

    // Fields
    syringe_rate_label_ = new QLabel("Rate (mL/min)", this);
    syringe_rate_label_->setWordWrap(true);
    syringe_rate_label_->setGeometry(0, 50, 50, 25);
    syringe_rate_field_ = new QLineEdit("", this);
    syringe_rate_field_->setGeometry(50, 50, 50, 25);

    return;
}

MainView::~MainView()
{
    delete ui;
}
