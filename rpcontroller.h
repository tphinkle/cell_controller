#ifndef RPCONTROLLER_H
#define RPCONTROLLER_H


// Program related
#include <mainmodel.h>
#include <mainview.h>
#include <rpthreadcontroller.h>


// Qt
#include <QTimer>

class RPController : public QObject
{
    Q_OBJECT

public:
    RPController(MainModel* main_model, MainView* main_view);
    ~RPController();

    QTimer* rp_plot_timer_;
    int rp_plot_period_;

    void setup_connections();

public slots:
    void receive_request_view_start_main_loop();
    void receive_request_view_set_threshold_multiplier();
    void receive_request_view_set_control_mode();
    void receive_request_record();





signals:

private:
    MainModel*  main_model_;
    MainView* main_view_;

    QThread* rp_thread_;
    RPThreadController* rp_thread_controller_;
};

#endif // RPCONTROLLER_H
