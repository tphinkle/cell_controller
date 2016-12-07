#include "mainmodel.h"

MainModel::MainModel()
{

}


SyringeModel& MainModel::syringe_model()
{
    return syringe_model_;
}

RPModel& MainModel::rp_model()
{
    return rp_model_;
}

CameraModel& MainModel::camera_model()
{
    return camera_model_;
}
