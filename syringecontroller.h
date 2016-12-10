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

public slots:
    // Model commands
    void command_model_set_local();
    void command_model_set_remote();
    void command_model_set_forward();
    void command_model_set_stop();
    void command_model_set_reverse();
    void command_model_switch_direction();
    void command_model_get_rate();
    void command_model_set_rate();

    // View commands
    void command_view_remote_button_up();
    void command_view_remote_button_down();
    void command_view_forward_button_up();
    void command_view_forward_button_down();
    void command_view_reverse_button_up();
    void command_view_reverse_button_down();
    void command_view_stop_button_up();
    void command_view_stop_button_down();
    void command_view_current_rate_field_change(double rate);


private:

    // Handles
    MainModel* main_model_;
    MainView* main_view_;




    // Methods



};

#endif // SYRINGECONTROLLER_H
