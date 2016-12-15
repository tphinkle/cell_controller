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

    setup_layout();

    return;
}

void MainView::setup_main_view()
{
    major_label_font_ = new QFont("Arial", 16, QFont::Bold);
    minor_label_font_ = new QFont("Arial", 14);
    button_font_ = new QFont("Arial", 12);
    checkbox_font_ = new QFont("Arial", 10);

    this->setWindowState(Qt::WindowMaximized);
    this->setWindowTitle("Cell controller");
    this->setFixedSize(1366, 768);



    return;
}

void MainView::setup_rp_view()
{
    // Plot and plot items
    rp_plot_ = new QwtPlot(QwtText("Resistive pulse data"), this);


    rp_control_label_ = new QLabel("RP Controls", this);
    rp_control_label_->setFont(*major_label_font_);

    rp_plot_curve_ = new QwtPlotCurve(QwtText("RP Data"));
    rp_plot_curve_->attach(rp_plot_);


    rp_baseline_mean_plot_curve_ = new QwtPlotCurve(QwtText("Baseline mean"));
    rp_baseline_mean_plot_curve_->setPen(* new QPen(Qt::red));
    rp_baseline_mean_plot_curve_->attach(rp_plot_);

    rp_baseline_lower_thresh_plot_curve_ = new QwtPlotCurve(QwtText("Baseline lower threshold"));
    rp_baseline_lower_thresh_plot_curve_->setPen(* new QPen(Qt::red));
    rp_baseline_lower_thresh_plot_curve_->attach(rp_plot_);

    rp_baseline_upper_thresh_plot_curve_ = new QwtPlotCurve(QwtText("Baseline upper threshold"));
    rp_baseline_upper_thresh_plot_curve_->setPen(* new QPen(Qt::red));
    rp_baseline_upper_thresh_plot_curve_->attach(rp_plot_);

    rp_plot_->setAxisTitle(QwtPlot::xBottom, "Sample");
    rp_plot_->setAxisTitle(QwtPlot::yLeft, "Current (xA)");


    rp_plot_->setAxisAutoScale(0, true);
    rp_plot_->setAxisAutoScale(QwtPlot::yLeft, true);

    rp_plot_->setAxisScale(QwtPlot::yLeft, -0.01, 0.01);

    // Buttons

    rp_start_button_ = new QPushButton("Start RP", this);
    rp_start_button_->setFont(*button_font_);


    rp_stop_button_ = new QPushButton("Stop RP", this);
    rp_stop_button_->setFont(*button_font_);

    rp_set_threshold_multiplier_button_ = new QPushButton("Threshold multiplier", this);
    rp_set_threshold_multiplier_button_->setFont(*button_font_);

    // Fields
    rp_threshold_multiplier_field_ = new QLineEdit("", this);
    rp_threshold_multiplier_field_->setFont(*minor_label_font_);


    // Checkboxes
    rp_control_syringe_checkbox_ = new QCheckBox("Syringe", this);
    rp_control_syringe_checkbox_->setFont(*checkbox_font_);


    rp_control_camera_checkbox_ = new QCheckBox("Camera", this);
    rp_control_camera_checkbox_->setFont(*checkbox_font_);


    return;
}

void MainView::setup_camera_view()
{

    // Image view
    camera_scene_ = new QGraphicsScene(this);
    camera_pixmapitem_ = new QGraphicsPixmapItem();
    camera_scene_->addItem(camera_pixmapitem_);

    camera_view_ = new QGraphicsView(this);
    camera_view_->setScene(camera_scene_);

    camera_view_->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    camera_view_->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);


    // Label
    camera_control_label_ = new QLabel("Camera Controls", this);
    camera_control_label_->setFont(*major_label_font_);

    // Buttons
    camera_start_button_ = new QPushButton("Start camera", this);
    camera_start_button_->setFont(*button_font_);


    camera_stop_button_ = new QPushButton("Stop camera", this);
    camera_stop_button_->setFont(*button_font_);





    return;
}

void MainView::setup_syringe_view()
{
    // Main label
    syringe_label_ = new QLabel("Syringe controls", this);
    syringe_label_->setFont(*major_label_font_);

    // Buttons
    syringe_local_button_ = new QPushButton("Local", this);
    syringe_local_button_->setFont(*button_font_);


    syringe_remote_button_ = new QPushButton("Remote", this);
    syringe_remote_button_->setFont(*button_font_);

    syringe_forward_button_ = new QPushButton("Forward", this);
    syringe_forward_button_->setFont(*button_font_);



    syringe_stop_button_ = new QPushButton("Stop", this);
    syringe_stop_button_->setFont(*button_font_);



    syringe_reverse_button_ = new QPushButton("Reverse", this);
    syringe_reverse_button_->setFont(*button_font_);



    syringe_switch_button_ = new QPushButton("Switch", this);
    syringe_switch_button_->setFont(*button_font_);

    syringe_get_rate_button_ = new QPushButton("Get", this);
    syringe_get_rate_button_->setFont(*button_font_);

    syringe_set_rate_button_ =  new QPushButton("Set", this);
    syringe_set_rate_button_->setFont(*button_font_);


    // Labels
    syringe_set_rate_label_ = new QLabel("Rate (mL/min)", this);
    syringe_set_rate_label_->setWordWrap(true);
    syringe_set_rate_label_->setFont(*minor_label_font_);


    syringe_current_rate_label_ = new QLabel("Current rate (mL/min)", this);
    syringe_current_rate_label_->setFont(*minor_label_font_);

    syringe_current_rate_field_ = new QLineEdit("", this);
    syringe_current_rate_field_->setFont(*minor_label_font_);
    syringe_current_rate_field_->setEnabled(false);

    // Fields


    syringe_set_rate_field_ = new QLineEdit("", this);
    syringe_set_rate_field_->setFont(*minor_label_font_);


    return;
}

void MainView::setup_layout()
{
    QWidget* main_widget = new QWidget();
    QWidget* syringe_widget = new QWidget();
    QWidget* plot_widget = new QWidget();
    QWidget* controls_widget = new QWidget();
    QWidget* stats_widget = new QWidget();

    setCentralWidget(main_widget);

    QVBoxLayout* main_layout = new QVBoxLayout(main_widget);
    main_layout->addWidget(syringe_widget);
    main_layout->addWidget(plot_widget);
    main_layout->addWidget(controls_widget);
    main_layout->addWidget(stats_widget);



    QHBoxLayout* syringe_layout = new QHBoxLayout(syringe_widget);
    syringe_layout->addWidget(syringe_label_);
    syringe_layout->addWidget(syringe_local_button_);
    syringe_layout->addWidget(syringe_remote_button_);
    syringe_layout->addWidget(syringe_forward_button_);
    syringe_layout->addWidget(syringe_stop_button_);
    syringe_layout->addWidget(syringe_reverse_button_);
    syringe_layout->addWidget(syringe_switch_button_);
    syringe_layout->addWidget(syringe_current_rate_label_);
    syringe_layout->addWidget(syringe_current_rate_field_);
    syringe_layout->addWidget(syringe_get_rate_button_);
    syringe_layout->addWidget(syringe_set_rate_label_);
    syringe_layout->addWidget(syringe_set_rate_field_);
    syringe_layout->addWidget(syringe_set_rate_button_);




    QHBoxLayout* plot_layout = new QHBoxLayout(plot_widget);
    plot_layout->addWidget(rp_plot_);
    plot_layout->addWidget(camera_view_);




    QHBoxLayout* controls_layout = new QHBoxLayout(controls_widget);
    controls_layout->addWidget(rp_control_label_);
    controls_layout->addWidget(rp_start_button_);
    controls_layout->addWidget(rp_stop_button_);
    controls_layout->addWidget(rp_set_threshold_multiplier_button_);
    controls_layout->addWidget(rp_threshold_multiplier_field_);
    controls_layout->addWidget(rp_control_syringe_checkbox_);
    controls_layout->addWidget(rp_control_camera_checkbox_);
    controls_layout->addWidget(camera_control_label_);
    controls_layout->addWidget(camera_start_button_);
    controls_layout->addWidget(camera_stop_button_);



    QHBoxLayout* stats_layout = new QHBoxLayout(stats_widget);
    stats_layout->addWidget(new QPushButton("Random stats button!"));



    return;
}

MainView::~MainView()
{
    delete ui;
}
