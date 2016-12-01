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

public slots:
    // Public methods
    void set_local();
    void set_remote();
    void set_forward();
    void set_stop();
    void set_reverse();
    void switch_direction();
    void set_rate();

private:

    // Handles
    MainModel* main_model_;
    MainView* main_view_;




    // Methods



};

#endif // SYRINGECONTROLLER_H
