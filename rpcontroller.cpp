#include "rpcontroller.h"
#include <QThread>

RPController::RPController(MainModel* main_model, MainView* main_view)
{
    main_model_ = main_model;
    main_view_ = main_view;


    rp_plot_period_ = (1000./60);
    rp_plot_timer_ = new QTimer();
    rp_plot_timer_->setInterval(rp_plot_period_);

}


void RPController::start_main_loop()
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
