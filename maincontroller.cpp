#include "maincontroller.h"

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
    // Syringe connections
    QObject::connect(main_view_->syringe_remote_button_, &QPushButton::clicked,\
                     &syringe_controller_, &SyringeController::set_remote);

    QObject::connect(main_view_->syringe_forward_button_, &QPushButton::clicked,\
                     &syringe_controller_, &SyringeController::set_forward);

    QObject::connect(main_view_->syringe_stop_button_, &QPushButton::clicked,\
                     &syringe_controller_, &SyringeController::set_stop);

    QObject::connect(main_view_->syringe_reverse_button_, &QPushButton::clicked,\
                     &syringe_controller_, &SyringeController::set_reverse);

    QObject::connect(main_view_->syringe_switch_button_, &QPushButton::clicked,\
                     &syringe_controller_, &SyringeController::switch_direction);

    QObject::connect(main_view_->syringe_set_rate_button_, &QPushButton::clicked,\
                     &syringe_controller_, &SyringeController::set_rate);

    // RP Connections
    QObject::connect(main_view_->rp_create_task_button_, &QPushButton::clicked,\
                     &rp_controller_, &RPController::create_DAQ_task);

    QObject::connect(main_view_->rp_start_task_button_, &QPushButton::clicked,\
                     &rp_controller_, &RPController::start_DAQ_task);


    return;
}

