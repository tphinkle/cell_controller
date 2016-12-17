#ifndef CAMERACONTROLLER_H
#define CAMERACONTROLLER_H

// Program specific
#include "mainmodel.h"
#include "mainview.h"

// Qt
#include <QTimer>

class CameraController : public QObject
{
    Q_OBJECT
public:
    CameraController(MainModel* main_model, MainView* main_view);

    MainModel* main_model_;
    MainView* main_view_;

    QThread* camera_thread_;

    void setup_connections();

    int camera_display_period_;
    QTimer* camera_display_timer_;

public slots:
    void receive_request_get_live_image();
    void receive_request_set_parameters();
    void receive_state_update_model_live_image(int res_x, int res_y);



signals:
    void command_model_get_live_image();
    void command_model_set_parameters(int frame_rate, int exposure_time, int res_x, int res_y);




};

#endif // CAMERACONTROLLER_H
