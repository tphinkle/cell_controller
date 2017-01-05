#ifndef RPTHREADCONTROLLER_H
#define RPTHREADCONTROLLER_H

// Qt
#include <QObject>

class RPThreadController : public QObject
{
    Q_OBJECT

public:
    RPThreadController();
    ~RPThreadController();

    // Get
    bool run();
    bool control_syringe();
    bool control_camera();
    double threshold_multiplier();



public slots:
    // Run
    void set_run(bool run);
    void stop_run();

    // Control syringe
    void set_control_syringe(bool control_syringe);

    // Control camera
    void set_control_camera(bool control_camera);


    // Threshold multiplier
    void set_threshold_multiplier(double threshold_multiplier);

private:
    bool run_;
    bool control_syringe_;
    bool control_camera_;
    double threshold_multiplier_;
};

#endif // RPTHREADCONTROLLER_H
