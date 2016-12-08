#ifndef CAMERACONTROLLER_H
#define CAMERACONTROLLER_H

// Program specific

// Qt
#include <QTimer>

class CameraController
{
public:
    CameraController();

    int camera_display_period_;
    QTimer* camera_display_timer_;

};

#endif // CAMERACONTROLLER_H
