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
    setup_camera_view();
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
    rp_plot_ = new QwtPlot(QwtText("Resistive pulse data"), this);
    rp_plot_->setGeometry(0, 500, 640, 480);


    rp_plot_curve_ = new QwtPlotCurve(QwtText("RP Data"));
    rp_plot_curve_->attach(rp_plot_);

    rp_plot_->setAxisTitle(QwtPlot::xBottom, "Sample");
    rp_plot_->setAxisTitle(QwtPlot::yLeft, "Current (xA)");


    rp_plot_->setAxisAutoScale(0, true);
    rp_plot_->setAxisAutoScale(QwtPlot::yLeft, true);

    rp_plot_->setAxisScale(QwtPlot::yLeft, -0.01, 0.01);

    // Buttons

    rp_start_button_ = new QPushButton("Start RP", this);
    rp_start_button_->setGeometry(0, 100, 50, 25);

    rp_stop_button_ = new QPushButton("Stop RP", this);
    rp_stop_button_->setGeometry(50, 100, 50, 25);

    // Checkboxes
    rp_control_syringe_checkbox_ = new QCheckBox("Syringe", this);
    rp_control_syringe_checkbox_->setGeometry(100, 100, 50, 25);

    rp_control_camera_checkbox_ = new QCheckBox("Camera", this);
    rp_control_camera_checkbox_->setGeometry(150, 100, 50, 25);

    return;
}

void MainView::setup_camera_view()
{
    // Buttons
    camera_start_button_ = new QPushButton("Start camera", this);
    camera_start_button_->setGeometry(0, 150, 50, 25);

    camera_stop_button_ = new QPushButton("Stop camera", this);
    camera_stop_button_->setGeometry(50, 150, 50, 25);


    // Image view
    scene_ = new QGraphicsScene(this);
    pixmapitem_ = new QGraphicsPixmapItem();
    scene_->addItem(pixmapitem_);

    view_ = new QGraphicsView(this);
    view_->setScene(scene_);
    view_->setGeometry(700, 500, 640, 480);
    view_->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    view_->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);

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
