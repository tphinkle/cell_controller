#include "maincontroller.h"
#include "syringecontroller.h"

MainController::MainController(MainModel* main_model, MainView* main_view) :
    syringe_controller_(main_model, main_view),
    rp_controller_(main_model, main_view)
{

    main_model_ = main_model;
    main_view_ = main_view;

    setup_connections();

}

void MainController::setup_connections()
{

    // Connect data pointers from Model to QwtPlot
    main_view_->rp_plot_curve_->setRawSamples(\
                &main_model_->rp_model().time_buffer_.front(),\
                &main_model_->rp_model().data_buffer_.front(),\
                main_model_->rp_model().data_buffer_.size()\
                );
    //main_model_->rp_model().buffer_



    // MainView -> RPController
    QObject::connect(main_view_->rp_start_button_, &QPushButton::clicked,\
                     &rp_controller_, &RPController::start_main_loop);

    // RPModel to SyringeController connections
    QObject::connect(&main_model_->rp_model(), SIGNAL(request_syringe_switch_direction()),\
                     &syringe_controller_, SLOT(command_model_switch_direction()));


    // Timers
    QObject::connect(rp_controller_.rp_plot_timer_, SIGNAL(timeout()), main_view_->rp_plot_, SLOT(replot()));
    QObject::connect(rp_controller_.rp_plot_timer_, SIGNAL(timeout()), rp_controller_.rp_plot_timer_, SLOT(start()));
    QObject::connect(main_view_->rp_start_button_, SIGNAL(clicked()), rp_controller_.rp_plot_timer_, SLOT(start()));



    return;
}

