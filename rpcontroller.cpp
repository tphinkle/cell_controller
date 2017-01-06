
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


    // Set up thread and thread controller

    rp_thread_ = NULL;
    rp_thread_controller_ = new RPThreadController();
    main_model_->rp_model().thread_controller_ = rp_thread_controller_;

}

RPController::~RPController()
{
    rp_thread_controller_->set_run(false);
    delete rp_thread_controller_;

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

    QObject::connect(main_view_->rp_set_threshold_multiplier_button_, SIGNAL(clicked()),
                     this, SLOT(receive_request_view_set_threshold_multiplier()));




    // Timers
    QObject::connect(main_view_->rp_start_button_, SIGNAL(clicked()), rp_plot_timer_, SLOT(start()));
    QObject::connect(rp_plot_timer_, SIGNAL(timeout()), main_view_->rp_plot_, SLOT(replot()));
    QObject::connect(rp_plot_timer_, SIGNAL(timeout()), this->rp_plot_timer_, SLOT(start()));




    return;
}



void RPController::receive_request_view_start_main_loop()
{

    rp_thread_controller_->set_run(true);
    rp_thread_ = new QThread();


    main_model_->rp_model().moveToThread(rp_thread_);

    ///////////////////////////
    // Control flow connections
    ///////////////////////////

    // Connect start thread to start main loop
    connect(rp_thread_, SIGNAL(started()), &main_model_->rp_model(), SLOT(start_main_loop()));

    // Connect end main loop to stop thread
    connect(main_view_->rp_stop_button_, SIGNAL(clicked()), rp_thread_controller_, SLOT(stop_run()));
    connect(main_view_->rp_stop_button_, SIGNAL(clicked()), rp_thread_, SLOT(quit()));

    // Connect stop thread to delete thread
    connect(rp_thread_, SIGNAL(finished()), rp_thread_, SLOT(deleteLater()));

    ///////////////////////////
    // Interactive connections
    ///////////////////////////


    rp_thread_->start();


    return;
}

void RPController::receive_request_view_set_threshold_multiplier()
{
    rp_thread_controller_->set_threshold_multiplier(main_view_->rp_threshold_multiplier_field_->text().toDouble());
    return;
}

