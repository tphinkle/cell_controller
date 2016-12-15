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
    void start_main_loop();
    void request_change_threshold_multiplier();

signals:
    void command_set_threshold_multiplier(double threshold_multiplier);

private:
    MainModel*  main_model_;
    MainView* main_view_;
};

#endif // RPCONTROLLER_H
