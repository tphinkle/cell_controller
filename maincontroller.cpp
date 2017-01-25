#include "maincontroller.h"
#include "syringecontroller.h"

MainController::MainController(MainModel* main_model, MainView* main_view) :
    syringe_controller_(main_model, main_view),
    rp_controller_(main_model, main_view),
    camera_controller_(main_model, main_view)
{

    main_model_ = main_model;
    main_view_ = main_view;



    syringe_controller_.setup_connections();

    rp_controller_.setup_connections();

    camera_controller_.setup_connections();

    setup_cross_connections();

}

MainController::~MainController()
{

}

void MainController::setup_cross_connections()
{
    // RPModel to SyringeController connections
    QObject::connect(&main_model_->rp_model(), SIGNAL(request_syringe_switch_direction()),\
                     &syringe_controller_, SLOT(receive_request_switch_direction()));

    // Dual record button
    QObject::connect(main_view_->dual_record_button_, SIGNAL(clicked()),
                     &rp_controller_, SLOT(receive_request_record()));

    QObject::connect(main_view_->dual_record_button_, SIGNAL(clicked()),
                     &camera_controller_, SLOT(receive_request_view_record()));






    return;
}

