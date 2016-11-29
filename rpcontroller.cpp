#include "rpcontroller.h"

RPController::RPController(MainModel* main_model, MainView* main_view)
{
    main_model_ = main_model;
    main_view_ = main_view;

}

void RPController::create_DAQ_task()
{
    main_model_->rp_model().create_DAQ_task();
    return;
}

void RPController::start_DAQ_task()
{
    main_model_->rp_model().start_DAQ_task();
    return;
}
