// Program specific
#include "cameracontroller.h"

CameraController::CameraController()
{
    camera_display_period_ = (1000./60);
    camera_display_timer_ = new QTimer();
    camera_display_timer_->setInterval(camera_display_period_);


}

