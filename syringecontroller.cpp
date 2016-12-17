// Program specific
#include "syringecontroller.h"

// Windows
#include <windows.h>

// Standard library
#include <iostream>
#include <sstream>

SyringeController::SyringeController(MainModel* main_model, MainView* main_view)
{
    main_model_ = main_model;
    main_view_ = main_view;




    syringe_thread_ = new QThread();
    main_model_->syringe_model().moveToThread(syringe_thread_);


    syringe_thread_->start();
}

void SyringeController::setup_connections()
{



    // View -> Controller
    QObject::connect(main_view_->syringe_remote_button_, &QPushButton::clicked,\
                     this, &SyringeController::receive_request_set_remote);

    QObject::connect(main_view_->syringe_forward_button_, &QPushButton::clicked,\
                     this, &SyringeController::receive_request_set_forward);

    QObject::connect(main_view_->syringe_stop_button_, &QPushButton::clicked,\
                     this, &SyringeController::receive_request_set_stop);

    QObject::connect(main_view_->syringe_reverse_button_, &QPushButton::clicked,\
                     this, &SyringeController::receive_request_set_reverse);

    QObject::connect(main_view_->syringe_switch_button_, &QPushButton::clicked,\
                     this, &SyringeController::receive_request_switch_direction);

    QObject::connect(main_view_->syringe_get_rate_button_, &QPushButton::clicked,\
                     this, &SyringeController::receive_request_get_rate);

    QObject::connect(main_view_->syringe_set_rate_button_, &QPushButton::clicked,\
                     this, &SyringeController::receive_request_set_rate);
    // Model-> Controller

    //
    QObject::connect(&main_model_->syringe_model(), SIGNAL(state_update_operating_mode_remote()),
                     this, SLOT(receive_state_update_model_operating_mode_remote() ));

    QObject::connect(&main_model_->syringe_model(), SIGNAL(state_update_operating_mode_remote()),
                     this, SLOT(receive_state_update_model_operating_mode_local() ));

\

    //
    QObject::connect(&main_model_->syringe_model(), SIGNAL(state_update_direction_forward()),
                     this, SLOT(receive_state_update_model_direction_forward()));

    QObject::connect(&main_model_->syringe_model(), SIGNAL(state_update_direction_reverse()),
                     this, SLOT(receive_state_update_model_direction_reverse()));


    //
    QObject::connect(&main_model_->syringe_model(), SIGNAL(state_update_motion_stopped()),
                     this, SLOT(receive_state_update_model_motion_stopped()));

    QObject::connect(&main_model_->syringe_model(), SIGNAL(state_update_motion_moving()),
                     this, SLOT(receive_state_update_model_motion_moving()));

    //


    QObject::connect(&main_model_->syringe_model(), &SyringeModel::state_update_rate,
                     this, &SyringeController::receive_state_update_model_rate);



    // Command connections

    QObject::connect(this, &SyringeController::command_syringe_set_local,
                     &main_model_->syringe_model(), &SyringeModel::syringe_set_local);

    QObject::connect(this, &SyringeController::command_syringe_set_remote,
                     &main_model_->syringe_model(), &SyringeModel::syringe_set_remote);

    QObject::connect(this, &SyringeController::command_syringe_set_forward,
                     &main_model_->syringe_model(), &SyringeModel::syringe_set_forward);

    QObject::connect(this, &SyringeController::command_syringe_set_stop,
                     &main_model_->syringe_model(), &SyringeModel::syringe_set_stop);

    QObject::connect(this, &SyringeController::command_syringe_set_reverse,
                     &main_model_->syringe_model(), &SyringeModel::syringe_set_reverse);

    QObject::connect(this, &SyringeController::command_syringe_switch_direction,
                     &main_model_->syringe_model(), &SyringeModel::syringe_switch_direction);

    QObject::connect(this, &SyringeController::command_syringe_get_rate,
                     &main_model_->syringe_model(), &SyringeModel::syringe_get_rate);

    QObject::connect(this, &SyringeController::command_syringe_set_rate,
                     &main_model_->syringe_model(), &SyringeModel::syringe_set_rate);



    return;
}

///////////////////////////////////////////////////////////////////////////////////////
/// RECEIVE VIEW REQUESTS
///////////////////////////////////////////////////////////////////////////////////////


void SyringeController::receive_request_set_local()
{
    emit command_syringe_set_local();
    return;
}

void SyringeController::receive_request_set_remote()
{
    emit command_syringe_set_remote();
    return;
}

void SyringeController::receive_request_set_forward()
{
    emit(command_syringe_set_forward());
    return;
}

void SyringeController::receive_request_set_stop()
{
    emit command_syringe_set_stop();
    return;
}

void SyringeController::receive_request_set_reverse()
{
    emit command_syringe_set_reverse();
    return;
}

void SyringeController::receive_request_get_rate()
{
    std::cout << "???" << std::endl;
    emit command_syringe_get_rate();
    return;
}

void SyringeController::receive_request_set_rate()
{
    // Get the rate from the rate field, convert to Windows encoded string
    std::string rate = main_view_->syringe_set_rate_field_->text().toLocal8Bit().constData();
    emit command_syringe_set_rate(rate);
    return;
}

void SyringeController::receive_request_switch_direction()
{
    emit command_syringe_switch_direction();
    return;
}

///////////////////////////////////////////////////////////////////////////////////////
/// MODEL STATE UPDATES
///////////////////////////////////////////////////////////////////////////////////////

void SyringeController::receive_state_update_model_operating_mode_local()
{
    main_view_->syringe_local_button_->setEnabled(false);
    main_view_->syringe_remote_button_->setEnabled(true);
    return;
}

void SyringeController::receive_state_update_model_operating_mode_remote()
{
    main_view_->syringe_local_button_->setEnabled(true);
    main_view_->syringe_remote_button_->setEnabled(false);
    return;
}

void SyringeController::receive_state_update_model_direction_forward()
{

    main_view_->syringe_stop_button_->setEnabled(true);
    main_view_->syringe_forward_button_->setEnabled(false);
    main_view_->syringe_reverse_button_->setEnabled(true);
    return;
}

void SyringeController::receive_state_update_model_direction_reverse()
{
    main_view_->syringe_stop_button_->setEnabled(true);
    main_view_->syringe_forward_button_->setEnabled(true);
    main_view_->syringe_reverse_button_->setEnabled(false);
    return;
}

void SyringeController::receive_state_update_model_motion_moving()
{
    main_view_->syringe_stop_button_->setEnabled(true);
    return;
}

void SyringeController::receive_state_update_model_motion_stopped()
{
    main_view_->syringe_forward_button_->setEnabled(true);
    main_view_->syringe_reverse_button_->setEnabled(true);
    main_view_->syringe_stop_button_->setEnabled(false);

    return;
}

void SyringeController::receive_state_update_model_rate(double rate)
{    
    std::ostringstream strs;
    strs << rate;
    std::string str = strs.str();
    main_view_->syringe_current_rate_field_->setText(QString::fromStdString(str));
    return;
}
