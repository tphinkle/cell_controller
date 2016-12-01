#ifndef RPTHREADCONTROLLER_H
#define RPTHREADCONTROLLER_H

// Qt
#include <QObject>

class RPThreadController : public QObject
{
    Q_OBJECT

public:
    RPThreadController(bool control_syringe, bool control_camera);

    // Get
    bool run();
    bool control_syringe();
    bool control_camera();



public slots:
    void stop_run();
    void set_control_syringe(bool control_syringe);
    void set_control_camera(bool control_camera);

private:
    bool run_;
    bool control_syringe_;
    bool control_camera_;
};

#endif // RPTHREADCONTROLLER_H
