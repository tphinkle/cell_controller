#ifndef MAINMODEL_H
#define MAINMODEL_H

#include <syringemodel.h>
#include <rpmodel.h>
#include <cameramodel.h>

class MainModel
{
public:
    MainModel();

    // Get/set
    SyringeModel& syringe_model();
    RPModel& rp_model();

private:

    // Handles
    SyringeModel syringe_model_;
    RPModel rp_model_;
    CameraModel camera_model_;



    // Methods


};

#endif // MAINMODEL_H
