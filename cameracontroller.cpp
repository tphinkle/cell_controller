// Program specific
#include "cameracontroller.h"

// Standard library
#include <iostream>

CameraController::CameraController(MainModel* main_model, MainView* main_view)
{
    main_model_ = main_model;
    main_view_ = main_view;

    camera_display_period_ = (1000./60);
    camera_display_timer_ = new QTimer();
    camera_display_timer_->setInterval(camera_display_period_);

    camera_thread_ = new QThread();

    main_model_->camera_model().moveToThread(camera_thread_);

    camera_thread_->start();

}

void CameraController::setup_connections()
{
    // Start button pressed
    // Timer starts
    // Timer times out, tells controller
    // Controller commands model to get a live image
    // Model gets a live image, tells the controller
    // Controller commands UI to update
    // Timer restarts


    // Start button -> start live view timer
    QObject::connect(main_view_->camera_start_button_, SIGNAL(clicked()), camera_display_timer_, SLOT(start()));


    // Timer timeout -> Request camera get live image
    QObject::connect(camera_display_timer_, SIGNAL(timeout()), this, SLOT(receive_request_get_live_image()));
    QObject::connect(camera_display_timer_, SIGNAL(timeout()), camera_display_timer_, SLOT(stop()));
    QObject::connect(this, SIGNAL(command_model_get_live_image()), &main_model_->camera_model(), SLOT(get_live_image()));


    // Receive new image -> Request update image
    QObject::connect(&main_model_->camera_model(), SIGNAL(state_update_live_image()), this, SLOT(receive_state_update_model_live_image()));


    // Timer finished -> Restart timer
    QObject::connect(camera_display_timer_, SIGNAL(timeout()), camera_display_timer_, SLOT(start()));



    return;
}


void CameraController::receive_request_get_live_image()
{
    emit command_model_get_live_image();
    return;
}

void CameraController::receive_state_update_model_live_image()
{
    std::cout << "Trying to plot live image." << std::endl;
    QImage img(main_model_->camera_model().live_image_pointer_, 640, 480, QImage::Format_Indexed8);
    main_view_->camera_scene_->clear();
    QPixmap pixmap = QPixmap::fromImage(img);
    main_view_->camera_scene_->addPixmap(pixmap);
    main_view_->camera_view_->update();


    return;
}


