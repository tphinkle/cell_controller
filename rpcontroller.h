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

public slots:
    void start_main_loop();
    void receive_request_update_plot();

private:
    MainModel*  main_model_;
    MainView* main_view_;
};

#endif // RPCONTROLLER_H
