#include "maincontroller.h"
#include "syringecontroller.h"

MainController::MainController(MainModel* main_model, MainView* main_view) :
    syringe_controller_(main_model, main_view),
    rp_controller_(main_model, main_view)
{

    main_model_ = main_model;
    main_view_ = main_view;

    setup_ui_connections();

    setup_cross_connections();

    syringe_controller_.setup_connections();

    rp_controller_.setup_connections();

    camera_controller_.setup_connections();

}

void MainController::setup_ui_connections()
{

    ////////////////////////////////////////////
    // Camera
    ////////////////////////////////////////////

    // Start button -> start live view timer
    QObject::connect(main_view_->camera_start_button_, SIGNAL(clicked()), camera_controller_.camera_display_timer_, SLOT(start()));

    // Timer timeout -> Update live view
    QObject::connect(camera_controller_.camera_display_timer_, SIGNAL(timeout()), camera_controller_.update_live_view_, SLOT(start()));

    // Receive new image -> Request update image
    QObject::connect(main_model_->camera_model(), SIGNAL(state_update_new_live_image()), camera_controller_.

    // Timer finished -> Restart timer
    QObject::connect(camera_controller_.camera_display_timer_, SIGNAL(timeout()), camera_controller_.camera_display_timer_, SLOT(start()));

                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                main_view_->camera_pixmapitem_
    main_view_->camera_image_->loadFromData(main_model_->camera_model().live_image_pointer_, 640*480, QImage::Format_Indexed8);
    main_view_->camera_scene_->clear();
    main_view_->camera_scene_->addPixmap(QPixmap::fromImage())


    ////////////////////////////////////////////
    // Resistive pulse (RP)
    ////////////////////////////////////////////

    QObject::connect(main_view_->rp_set_threshold_multiplier_button_, &QPushButton::clicked,\
                     &rp_controller_, &RPController::request_change_threshold_multiplier, Qt::QueuedConnection);

    QObject::connect(main_view_->rp_start_button_, &QPushButton::clicked,\
                     &rp_controller_, &RPController::start_main_loop);
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
    QObject::connect(main_view_->rp_start_button_, SIGNAL(clicked()), rp_controller_.rp_plot_timer_, SLOT(start()));
    QObject::connect(rp_controller_.rp_plot_timer_, SIGNAL(timeout()), main_view_->rp_plot_, SLOT(replot()));
    QObject::connect(rp_controller_.rp_plot_timer_, SIGNAL(timeout()), rp_controller_.rp_plot_timer_, SLOT(start()));




    return;
}

void MainController::setup_cross_connections()
{
    // RPModel to SyringeController connections
    QObject::connect(&main_model_->rp_model(), SIGNAL(request_syringe_switch_direction()),\
                     &syringe_controller_, SLOT(command_model_switch_direction()));

    return;
}

