// Program specific
#include "cameracontroller.h"

// Standard library
#include <iostream>

CameraController::CameraController(MainModel* main_model, MainView* main_view)
{
    main_model_ = main_model;
    main_view_ = main_view;

    camera_display_period_ = (1000./30);
    camera_display_timer_ = new QTimer();
    camera_display_timer_->setInterval(camera_display_period_);

    camera_thread_ = new QThread();
    main_model_->camera_model().moveToThread(camera_thread_);

    camera_thread_->start();

    setup_connections();

}

CameraController::~CameraController()
{

    //delete camera_thread_;
    delete camera_display_timer_;
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

    // Set parameters abutton
    QObject::connect(main_view_->camera_set_parameters_button_, SIGNAL(clicked()), this, SLOT(receive_request_set_parameters()));
    QObject::connect(this, &CameraController::command_model_set_default_all_parameters, &main_model_->camera_model(), &CameraModel::set_default_all_parameters);



    // Start button -> start live view timer
    QObject::connect(main_view_->camera_start_button_, SIGNAL(clicked()), camera_display_timer_, SLOT(start()));

    QObject::connect(main_view_->camera_record_button_, SIGNAL(clicked()), this, SLOT(receive_request_view_record()));



    // Timer timeout -> Request camera get live image
    QObject::connect(camera_display_timer_, SIGNAL(timeout()), this, SLOT(receive_request_get_live_image()));
    QObject::connect(camera_display_timer_, SIGNAL(timeout()), camera_display_timer_, SLOT(stop()));
    QObject::connect(this, SIGNAL(command_model_get_live_image()), &main_model_->camera_model(), SLOT(get_live_image()));


    // Receive new image -> Request update image
    QObject::connect(&main_model_->camera_model(), &CameraModel::state_update_live_image, this, &CameraController::receive_state_update_model_live_image);



    // Stop button
    QObject::connect(main_view_->camera_stop_button_, SIGNAL(clicked()), camera_display_timer_, SLOT(stop()));


    // Request get cine info button
    QObject::connect(main_view_->camera_get_cine_info_button_, SIGNAL(clicked()), this, SLOT(receive_request_view_get_cine_info()));


    QObject::connect(this, SIGNAL(command_model_record()), &main_model_->camera_model(), SLOT(record()));

    camera_thread_ = new QThread();
    //main_model_->camera_model().moveToThread(camera_thread_);

    //camera_thread_->start();



    return;
}



void CameraController::receive_request_view_record()
{
    //camera_display_timer_->stop();
    //QTimer* record_timer = new QTimer();
    //record_timer->setInterval(1000.*main_model_->camera_model().num_images()/main_model_->camera_model().frame_rate());
    //QObject::connect(record_timer, SIGNAL(timeout()), this, SIGNAL(command_model_record()));
    //QObject::connect(record_timer, SIGNAL(timeout()), record_timer, SLOT(stop()));
    //QObject::connect(record_timer, SIGNAL(timeout()), record_timer, SLOT(deleteLater()));
    //record_timer->start();

    //QObject::connect(camera_display_timer_, SIGNAL(timeout()), this, SLOT(receive_request_get_live_image()));
    //QObject::connect(camera_display_timer_, SIGNAL(timeout()), camera_display_timer_, SLOT(stop()));


    emit command_model_record();

    return;
}


void CameraController::receive_request_view_get_cine_info()
{
    main_model_->camera_model().get_cine_info();
    return;
}


void CameraController::receive_request_get_live_image()
{
    emit command_model_get_live_image();
    return;
}

void CameraController::receive_request_set_parameters()
{
    int frame_rate = main_view_->camera_frame_rate_field_->text().toDouble();
    int exposure_time = main_view_->camera_exposure_time_field_->text().toDouble();


    QString res = main_view_->camera_resolution_combo_box_->currentText();
    int res_x = res.split("x")[0].toDouble();
    int res_y = res.split("x")[1].toDouble();

    int num_images = 3000;

    std::cout << "frame_rate: " << frame_rate << std::endl;
    std::cout << "exposure_time: " << exposure_time << std::endl;
    std::cout << "res_x: " << res_x << std::endl;
    std::cout << "res_y: " << res_y << std::endl;
    std::cout << "num_images: " << num_images << std::endl;


    emit command_model_set_default_all_parameters(frame_rate, exposure_time, res_x, res_y, num_images);

    return;
}

void CameraController::receive_state_update_model_live_image(int res_x, int res_y)
{
    QImage img(main_model_->camera_model().live_image_pointer_, res_x, res_y, QImage::Format_Indexed8);
    main_view_->camera_scene_->clear();
    QPixmap pixmap = QPixmap::fromImage(img);
    main_view_->camera_scene_->addPixmap(pixmap);
    main_view_->camera_view_->update();

    camera_display_timer_->start();


    return;
}


