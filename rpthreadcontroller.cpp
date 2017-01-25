// Standard library
#include <iostream>

// Program specific
#include "rpthreadcontroller.h"



RPThreadController::RPThreadController()
{

    set_run(false);
    set_threshold_multiplier(100);
    control_syringe_ = false;
    control_camera_ = false;
    save_buffer_ = false;



}

RPThreadController::~RPThreadController()
{
    set_run(false);
}

////////////////////////////////
//
// Run
//
////////////////////////////////

void RPThreadController::set_run(bool run)
{
    run_ = run;
    return;
}

void RPThreadController::stop_run()
{
    run_ = false;
    return;
}

bool RPThreadController::run()
{
    return run_;
}

void RPThreadController::set_control_mode(bool control_mode)
{
    set_control_syringe(control_mode);
    set_control_camera(control_mode);
    return;
}

///////////////////////////////
//
// Control syringe
//
///////////////////////////////

void RPThreadController::set_control_syringe(bool control_syringe)
{
    control_syringe_ = control_syringe;
    return;
}

bool RPThreadController::control_syringe()
{
    return control_syringe_;
}


//////////////////////////////////
//
// Control camera
//
//////////////////////////////////

void RPThreadController::set_control_camera(bool control_camera)
{
    control_camera_ = control_camera;
    return;
}


bool RPThreadController::control_camera()
{
    return control_camera_;
}


//////////////////////////////////
//
// Threshold multiplier
//
//////////////////////////////////

double RPThreadController::threshold_multiplier()
{
    return threshold_multiplier_;
}

void RPThreadController::set_threshold_multiplier(double threshold_multiplier)
{
    threshold_multiplier_ = threshold_multiplier;
    return;
}

//////////////////////////////////
//
// Save buffer
//
//////////////////////////////////

bool RPThreadController::save_buffer()
{
    return save_buffer_;
}

void RPThreadController::set_save_buffer(bool save_buffer)
{
   save_buffer_ = save_buffer;
   return;
}




