
// Standard library
#include <iostream>

// Program specific
#include "rpcontroller.h"

// Qt
#include <QThread>

RPController::RPController(MainModel* main_model, MainView* main_view)
{
    main_model_ = main_model;
    main_view_ = main_view;


    rp_plot_period_ = (1000./60);
    rp_plot_timer_ = new QTimer();
    rp_plot_timer_->setInterval(rp_plot_period_);

}

void RPController::setup_connections()
{
    // Shared buffers
    main_view_->rp_plot_curve_->setRawSamples(\
                &main_model_->rp_model().time_buffer_.front(),\
                &main_model_->rp_model().data_buffer_.front(),\
                main_model_->rp_model().data_buffer_.size()\
                );

    main_view_->rp_baseline_mean_plot_curve_->setRawSamples(\
                &main_model_->rp_model().baseline_time_buffer_.front(),
                &main_model_->rp_model().baseline_mean_buffer_.front(),
                2);

    main_view_->rp_baseline_lower_thresh_plot_curve_->setRawSamples(\
                &main_model_->rp_model().baseline_time_buffer_.front(),
                &main_model_->rp_model().baseline_lower_thresh_buffer_.front(),
                2);

    main_view_->rp_baseline_upper_thresh_plot_curve_->setRawSamples(\
                &main_model_->rp_model().baseline_time_buffer_.front(),
                &main_model_->rp_model().baseline_upper_thresh_buffer_.front(),
                2);


    // View requests
    QObject::connect(main_view_->rp_start_button_, SIGNAL(clicked()),\
                     this, SLOT(receive_request_view_start_main_loop()));

    QObject::connect(main_view_->rp_increase_scale_button_, SIGNAL(clicked()),
                     this, SLOT(increase_main_plot_range()));

    QObject::connect(main_view_->rp_decrease_scale_button_, SIGNAL(clicked()),
                     this, SLOT(decrease_main_plot_range()));


    // Timers
    QObject::connect(main_view_->rp_start_button_, SIGNAL(clicked()), rp_plot_timer_, SLOT(start()));
    QObject::connect(rp_plot_timer_, SIGNAL(timeout()), main_view_->rp_plot_, SLOT(replot()));
    QObject::connect(rp_plot_timer_, SIGNAL(timeout()), this->rp_plot_timer_, SLOT(start()));




    return;
}



void RPController::receive_request_view_start_main_loop()
{
    QThread* rp_thread = new QThread();
    RPThreadController* rp_thread_controller = new RPThreadController(\
                main_view_->rp_control_syringe_checkbox_->isChecked(), \
                main_view_->rp_control_camera_checkbox_->isChecked());

    main_model_->rp_model().thread_controller_ = rp_thread_controller;

    main_model_->rp_model().moveToThread(rp_thread);

    ///////////////////////////
    // Control flow connections
    ///////////////////////////

    // Connect start thread to start main loop
    connect(rp_thread, SIGNAL(started()), &main_model_->rp_model(), SLOT(start_main_loop()));

    // Connect end main loop to stop thread
    connect(main_view_->rp_stop_button_, SIGNAL(clicked()), rp_thread_controller, SLOT(stop_run()));
    connect(main_view_->rp_stop_button_, SIGNAL(clicked()), rp_thread, SLOT(quit()));

    // Connect stop thread to delete thread
    connect(rp_thread, SIGNAL(finished()), rp_thread, SLOT(deleteLater()));

    ///////////////////////////
    // Interactive connections
    ///////////////////////////

    connect(main_view_->rp_control_syringe_checkbox_, SIGNAL(clicked(bool)),\
            rp_thread_controller, SLOT(set_control_syringe(bool)));




    rp_thread->start();


    return;
}

void RPController::increase_main_plot_range()
{
    QwtInterval axis_interval = main_view_->rp_plot_->axisInterval(0);
    float old_range = axis_interval.maxValue() - axis_interval.minValue();
    float new_range = 1.5*old_range;

    float new_y_lower = (axis_interval.maxValue()+axis_interval.minValue())/2. - new_range/2.;
    float new_y_upper = (axis_interval.maxValue()+axis_interval.minValue())/2. + new_range/2.;

    std::cout << "old limits" << axis_interval.minValue() << "\t\t\t" << axis_interval.maxValue() << std::endl;

    std::cout << new_y_lower << "\t\t\t" << new_y_upper << std::endl;

    main_view_->rp_plot_->setAxisScale(QwtPlot::yLeft, new_y_lower, new_y_upper);
    return;
}

void RPController::decrease_main_plot_range()
{
    QwtInterval axis_interval = main_view_->rp_plot_->axisInterval(0);
    float old_range = axis_interval.maxValue() - axis_interval.minValue();
    float new_range = 0.5*old_range;

    float new_y_lower = (axis_interval.maxValue()+axis_interval.minValue())/2 - new_range/2.;
    float new_y_upper = (axis_interval.maxValue()+axis_interval.minValue())/2 + new_range/2.;

    std::cout << new_y_lower << "\t\t\t" << new_y_upper << std::endl;

    main_view_->rp_plot_->setAxisScale(QwtPlot::yLeft, new_y_lower, new_y_upper);
    return;
}
