// Standard library
#include <iostream>

// Program specific
#include "rpthreadcontroller.h"



RPThreadController::RPThreadController(bool control_syringe, bool control_camera)
{

    run_ = true;
    control_syringe_ = control_syringe;
    control_camera_ = control_camera;



}

void RPThreadController::stop_run()
{
    run_ = false;
    return;
}

void RPThreadController::set_control_syringe(bool control_syringe)
{
    control_syringe_ = control_syringe;
    return;
}

void RPThreadController::set_control_camera(bool control_camera)
{
    control_camera_ = control_camera;
    return;
}

bool RPThreadController::run()
{
    return run_;
}

bool RPThreadController::control_syringe()
{
    return control_syringe_;
}

bool RPThreadController::control_camera()
{
    return control_camera_;
}
