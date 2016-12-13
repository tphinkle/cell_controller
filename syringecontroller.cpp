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

    setup_connections();
}

void SyringeController::setup_connections()
{
    // View -> Controller
    QObject::connect(main_view_->syringe_remote_button_, &QPushButton::clicked,\
                     this, &SyringeController::command_model_set_remote);

    QObject::connect(main_view_->syringe_forward_button_, &QPushButton::clicked,\
                     this, &SyringeController::command_model_set_forward);

    QObject::connect(main_view_->syringe_stop_button_, &QPushButton::clicked,\
                     this, &SyringeController::command_model_set_stop);

    QObject::connect(main_view_->syringe_reverse_button_, &QPushButton::clicked,\
                     this, &SyringeController::command_model_set_reverse);

    QObject::connect(main_view_->syringe_switch_button_, &QPushButton::clicked,\
                     this, &SyringeController::command_model_switch_direction);

    QObject::connect(main_view_->syringe_get_rate_button_, &QPushButton::clicked,\
                     this, &SyringeController::command_model_get_rate);

    QObject::connect(main_view_->syringe_set_rate_button_, &QPushButton::clicked,\
                     this, &SyringeController::command_model_set_rate);

    // Model-> Controller

    //
    QObject::connect(&main_model_->syringe_model(), SIGNAL(state_update_operating_mode_remote()),
                     this, SLOT(command_view_remote_button_down()));

    QObject::connect(&main_model_->syringe_model(), SIGNAL(state_update_operating_mode_remote()),
                     this, SLOT(command_view_local_button_up()));

    //
    QObject::connect(&main_model_->syringe_model(), SIGNAL(state_update_operating_mode_local()),
                     this, SLOT(command_view_local_button_down()));

    QObject::connect(&main_model_->syringe_model(), SIGNAL(state_update_operating_mode_local()),
                     this, SLOT(command_view_remote_button_up()));

    //
    QObject::connect(&main_model_->syringe_model(), SIGNAL(state_update_direction_forward()),
                     this, SLOT(command_view_forward_button_down()));

    QObject::connect(&main_model_->syringe_model(), SIGNAL(state_update_direction_forward()),
                     this, SLOT(command_view_reverse_button_up()));

    //
    QObject::connect(&main_model_->syringe_model(), SIGNAL(state_update_direction_reverse()),
                     this, SLOT(command_view_reverse_button_down()));

    QObject::connect(&main_model_->syringe_model(), SIGNAL(state_update_direction_reverse()),
                     this, SLOT(command_view_forward_button_up()));

    //
    QObject::connect(&main_model_->syringe_model(), SIGNAL(state_update_motion_stopped()),
                     this, SLOT(command_view_stop_button_down()));

    QObject::connect(&main_model_->syringe_model(), SIGNAL(state_update_motion_stopped()),
                     this, SLOT(command_view_forward_button_up()));

    QObject::connect(&main_model_->syringe_model(), SIGNAL(state_update_motion_stopped()),
                     this, SLOT(command_view_reverse_button_up()));

    //
    QObject::connect(&main_model_->syringe_model(), SIGNAL(state_update_motion_moving()),
                     this, SLOT(command_view_stop_button_up()));

    QObject::connect(&main_model_->syringe_model(), &SyringeModel::state_update_rate,
                     this, &SyringeController::command_view_current_rate_field_change);






    return;
}

///////////////////////////////////////////////////////////////////////////////////////
/// MODEL COMMANDS
///////////////////////////////////////////////////////////////////////////////////////


void SyringeController::command_model_set_local()
{
    main_model_->syringe_model().syringe_set_local();
    return;
}

void SyringeController::command_model_set_remote()
{
    main_model_->syringe_model().syringe_set_remote();
    return;
}

void SyringeController::command_model_set_forward()
{
    main_model_->syringe_model().syringe_set_forward();
    return;
}

void SyringeController::command_model_set_stop()
{
    main_model_->syringe_model().syringe_set_stop();
    return;
}

void SyringeController::command_model_set_reverse()
{
    main_model_->syringe_model().syringe_set_reverse();
    return;
}

void SyringeController::command_model_get_rate()
{
    main_model_->syringe_model().syringe_get_rate();
    return;
}

void SyringeController::command_model_set_rate()
{
    // Get the rate from the rate field, convert to Windows encoded string
    std::string rate = main_view_->syringe_set_rate_field_->text().toLocal8Bit().constData();
    main_model_->syringe_model().syringe_set_rate(rate);
    return;
}

void SyringeController::command_model_switch_direction()
{
    main_model_->syringe_model().syringe_switch_direction();
    return;
}

///////////////////////////////////////////////////////////////////////////////////////
/// VIEW COMMANDS
///////////////////////////////////////////////////////////////////////////////////////

void SyringeController::command_view_remote_button_up()
{
    main_view_->syringe_local_button_->setEnabled(true);
    return;
}

void SyringeController::command_view_remote_button_down()
{
    main_view_->syringe_local_button_->setEnabled(false);
    return;
}

void SyringeController::command_view_remote_button_up()
{
    main_view_->syringe_remote_button_->setEnabled(true);
    return;
}

void SyringeController::command_view_remote_button_down()
{
    main_view_->syringe_remote_button_->setEnabled(false);
    return;
}

void SyringeController::command_view_forward_button_up()
{
    main_view_->syringe_forward_button_->setEnabled(true);
    return;
}

void SyringeController::command_view_forward_button_down()
{
    main_view_->syringe_forward_button_->setEnabled(false);
    return;
}

void SyringeController::command_view_reverse_button_up()
{
    main_view_->syringe_reverse_button_->setEnabled(true);
    return;
}

void SyringeController::command_view_reverse_button_down()
{
    main_view_->syringe_reverse_button_->setEnabled(false);
    return;
}

void SyringeController::command_view_stop_button_up()
{
    main_view_->syringe_stop_button_->setEnabled(true);
    return;
}

void SyringeController::command_view_stop_button_down()
{
    main_view_->syringe_stop_button_->setEnabled(false);
    return;
}

void SyringeController::command_view_current_rate_field_change(double rate)
{
    std::ostringstream strs;
    strs << rate;
    std::string str = strs.str();
    main_view_->syringe_current_rate_field_->setText(QString::fromStdString(str));
    return;
}
