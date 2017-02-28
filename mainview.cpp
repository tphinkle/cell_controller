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
    setup_miscellaneous_view();

    setup_layout();

    return;
}

void MainView::setup_main_view()
{
    major_label_font_ = new QFont("Arial", 16, QFont::Bold);
    minor_label_font_ = new QFont("Arial", 14);
    button_font_ = new QFont("Arial", 12);
    // checkbox_font_ = new QFont("Arial", 10);

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
    //rp_plot_curve_->setRenderHint(QwtPlotItem::RenderAntialiased, false);

    rp_plot_panner_ = new QwtPlotPanner(rp_plot_->canvas());
    rp_plot_panner_->setAxisEnabled(QwtPlot::yLeft, true);
    rp_plot_panner_->setAxisEnabled(QwtPlot::xBottom, false);

    rp_plot_magnifier_ = new QwtPlotMagnifier(rp_plot_->canvas());
    rp_plot_magnifier_->setAxisEnabled(QwtPlot::yLeft, true);
    rp_plot_magnifier_->setAxisEnabled(QwtPlot::xBottom, false);



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


    //rp_plot_->setAxisAutoScale(0, true);
    rp_plot_->setAxisAutoScale(QwtPlot::yLeft, true);
    rp_plot_->axisScaleEngine(QwtPlot::xBottom)->setAttribute(QwtScaleEngine::Floating, true); // For auto scaling if I < 0



    // Buttons

    rp_start_button_ = new QPushButton("Start RP", this);
    rp_start_button_->setFont(*button_font_);


    rp_stop_button_ = new QPushButton("Stop RP", this);
    rp_stop_button_->setFont(*button_font_);

    rp_set_control_mode_button_ = new QPushButton("Syringe control", this);
    rp_set_control_mode_button_->setFont(*button_font_);

    rp_set_threshold_multiplier_button_ = new QPushButton("Threshold multiplier", this);
    rp_set_threshold_multiplier_button_->setFont(*button_font_);

    // Fields
    rp_threshold_multiplier_field_ = new QLineEdit("", this);
    rp_threshold_multiplier_field_->setFont(*minor_label_font_);



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

    // ComboBox
    camera_resolution_combo_box_ = new QComboBox(this);
    camera_resolution_combo_box_->addItem("640x480");
    camera_resolution_combo_box_->addItem("320x240");

    // Label
    camera_control_label_ = new QLabel("Camera Controls", this);
    camera_control_label_->setFont(*major_label_font_);

    camera_resolution_label_ = new QLabel("Resolution (pix squared)", this);
    camera_resolution_label_->setFont(*minor_label_font_);

    camera_frame_rate_label_ = new QLabel("Frame rate (S/s)", this);
    camera_frame_rate_label_->setFont(*minor_label_font_);

    camera_exposure_time_label_ = new QLabel("Exposure time (us)", this);
    camera_exposure_time_label_->setFont(*minor_label_font_);

    // Fields
    camera_frame_rate_field_ = new QLineEdit("", this);
    camera_frame_rate_field_->setFont(*minor_label_font_);
    camera_frame_rate_field_->setText("50000");

    camera_exposure_time_field_ = new QLineEdit("", this);
    camera_exposure_time_field_->setFont(*minor_label_font_);
    camera_exposure_time_field_->setText("5");

    // Buttons
    camera_start_button_ = new QPushButton("Start camera", this);
    camera_start_button_->setFont(*button_font_);


    camera_stop_button_ = new QPushButton("Stop camera", this);
    camera_stop_button_->setFont(*button_font_);

    camera_record_button_ = new QPushButton("Record cine", this);
    camera_record_button_->setFont(*button_font_);

    camera_set_frame_rate_button_ = new QPushButton("Set", this);
    camera_set_frame_rate_button_->setFont(*button_font_);

    camera_set_exposure_time_button_ = new QPushButton("Set", this);
    camera_set_exposure_time_button_->setFont(*button_font_);

    camera_set_parameters_button_ = new QPushButton("Set parameters", this);
    camera_set_parameters_button_->setFont(*button_font_);

    camera_get_cine_info_button_ = new QPushButton("Get cine info", this);
    camera_get_cine_info_button_->setFont(*button_font_);





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

void MainView::setup_miscellaneous_view()
{
    dual_record_button_ = new QPushButton("Dual record", this);
    dual_record_button_->setFont(*button_font_);
}

void MainView::setup_layout()
{
    QWidget* main_widget = new QWidget();
    QWidget* syringe_widget = new QWidget();
    QWidget* plot_widget = new QWidget();
    QWidget* rp_controls_widget = new QWidget();
    QWidget* camera_controls_widget = new QWidget();
    QWidget* miscellaneous_controls_widget = new QWidget();

    setCentralWidget(main_widget);

    QVBoxLayout* main_layout = new QVBoxLayout(main_widget);
    main_layout->addWidget(syringe_widget);
    main_layout->addWidget(plot_widget);
    main_layout->addWidget(rp_controls_widget);
    main_layout->addWidget(camera_controls_widget);
    main_layout->addWidget(miscellaneous_controls_widget);



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




    QHBoxLayout* rp_controls_layout = new QHBoxLayout(rp_controls_widget);
    rp_controls_layout->addWidget(rp_control_label_);
    rp_controls_layout->addWidget(rp_start_button_);
    rp_controls_layout->addWidget(rp_stop_button_);
    rp_controls_layout->addWidget(rp_set_control_mode_button_);
    rp_controls_layout->addWidget(rp_set_threshold_multiplier_button_);
    rp_controls_layout->addWidget(rp_threshold_multiplier_field_);




    QHBoxLayout* camera_controls_layout = new QHBoxLayout(camera_controls_widget);
    camera_controls_layout->addWidget(camera_control_label_);
    camera_controls_layout->addWidget(camera_start_button_);
    camera_controls_layout->addWidget(camera_stop_button_);
    camera_controls_layout->addWidget(camera_record_button_);
    camera_controls_layout->addWidget(camera_resolution_label_);
    camera_controls_layout->addWidget(camera_resolution_combo_box_);

    camera_controls_layout->addWidget(camera_frame_rate_label_);
    camera_controls_layout->addWidget(camera_frame_rate_field_);
    camera_controls_layout->addWidget(camera_set_frame_rate_button_);

    camera_controls_layout->addWidget(camera_exposure_time_label_);
    camera_controls_layout->addWidget(camera_exposure_time_field_);
    camera_controls_layout->addWidget(camera_set_exposure_time_button_);

    camera_controls_layout->addWidget(camera_set_parameters_button_);
    camera_controls_layout->addWidget(camera_get_cine_info_button_);



    QHBoxLayout*miscellaneous_controls_layout = new QHBoxLayout(miscellaneous_controls_widget);
    miscellaneous_controls_layout->addWidget(dual_record_button_);




    return;
}

MainView::~MainView()
{
    delete ui;
}
