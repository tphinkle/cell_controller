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
