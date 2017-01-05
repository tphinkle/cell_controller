#ifndef RPCONTROLLER_H
#define RPCONTROLLER_H


// Program related
#include <mainmodel.h>
#include <mainview.h>

// Qt
#include <QTimer>

class RPController : public QObject
{
    Q_OBJECT

public:
    RPController(MainModel* main_model, MainView* main_view);

    QTimer* rp_plot_timer_;
    int rp_plot_period_;

    void setup_connections();

public slots:
    void receive_request_view_start_main_loop();
    void increase_main_plot_range();
    void decrease_main_plot_range();



signals:

private:
    MainModel*  main_model_;
    MainView* main_view_;
};

#endif // RPCONTROLLER_H
