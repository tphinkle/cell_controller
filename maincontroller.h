#ifndef MAINCONTROLLER_H
#define MAINCONTROLLER_H


// Program spsecific
#include <mainmodel.h>
#include <mainview.h>
#include <syringecontroller.h>
#include <rpcontroller.h>
#include <cameracontroller.h>

class MainController : public QObject
{
    Q_OBJECT

public:
    MainController(MainModel* main_model, MainView* main_view);

    void setup_ui_connections();
    void setup_cross_connections();


private:
    SyringeController syringe_controller_;
    RPController rp_controller_;
    CameraController camera_controller_;

    // Handles
    MainModel* main_model_;
    MainView* main_view_;

};

#endif // MAINCONTROLLER_H
