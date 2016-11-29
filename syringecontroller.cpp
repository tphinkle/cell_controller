#include "syringecontroller.h"
#include <windows.h>

#include <iostream>
#include <sstream>

SyringeController::SyringeController(MainModel* main_model, MainView* main_view)
{
    main_model_ = main_model;
    main_view_ = main_view;
}

void SyringeController::set_local()
{
    main_model_->syringe_model().set_local();
    return;
}

void SyringeController::set_remote()
{
    main_model_->syringe_model().set_remote();
    return;
}

void SyringeController::set_forward()
{
    main_model_->syringe_model().set_forward();
    return;
}

void SyringeController::set_stop()
{
    main_model_->syringe_model().set_stop();
    return;
}

void SyringeController::set_reverse()
{
    main_model_->syringe_model().set_reverse();
    return;
}

void SyringeController::set_rate()
{
    // Get the rate from the rate field, convert to Windows encoded string
    std::string rate = main_view_->syringe_rate_field_->text().toLocal8Bit().constData();
    main_model_->syringe_model().set_rate(rate);
    return;
}

void SyringeController::switch_direction()
{
    main_model_->syringe_model().switch_direction();
    return;
}
