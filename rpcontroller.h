#ifndef RPCONTROLLER_H
#define RPCONTROLLER_H

#include <mainmodel.h>
#include <mainview.h>

class RPController : public QObject
{
    Q_OBJECT

public:
    RPController(MainModel* main_model, MainView* main_view);

public slots:
    void create_DAQ_task();
    void start_DAQ_task();

private:
    MainModel*  main_model_;
    MainView* main_view_;
};

#endif // RPCONTROLLER_H
