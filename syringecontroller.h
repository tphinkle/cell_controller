#ifndef SYRINGECONTROLLER_H
#define SYRINGECONTROLLER_H

// Windows
#include <windows.h>

// Program specific
#include <mainmodel.h>
#include <mainview.h>




class SyringeController : public QObject
{
    Q_OBJECT

public:
    SyringeController(MainModel* main_model, MainView* main_view);

    void setup_connections();

    QThread* syringe_thread_;

public slots:
    // Model commands
    void receive_request_set_local();
    void receive_request_set_remote();
    void receive_request_set_forward();
    void receive_request_set_stop();
    void receive_request_set_reverse();
    void receive_request_switch_direction();
    void receive_request_get_rate();
    void receive_request_set_rate();

    // View commands
    void receive_state_update_model_operating_mode_local();
    void receive_state_update_model_operating_mode_remote();
    void receive_state_update_model_direction_forward();
    void receive_state_update_model_direction_reverse();
    void receive_state_update_model_motion_moving();
    void receive_state_update_model_motion_stopped();
    void receive_state_update_model_rate(double rate);

    // Syringe commands

signals:
    void command_syringe_set_local();
    void command_syringe_set_remote();

    void command_syringe_set_forward();
    void command_syringe_set_stop();
    void command_syringe_set_reverse();

    void command_syringe_switch_direction();

    void command_syringe_get_rate();
    void command_syringe_set_rate(std::string rate);



private:

    // Handles
    MainModel* main_model_;
    MainView* main_view_;




    // Methods



};

#endif // SYRINGECONTROLLER_H
