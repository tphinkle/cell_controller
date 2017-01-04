#ifndef SYRINGEMODEL_H
#define SYRINGEMODEL_H

// Standard library
#include <string>

// Project specific
#include <serialconnection.h>

// Qt
#include <QObject>

class SyringeModel : public QObject
{
    Q_OBJECT


public:

    SyringeModel();

    // State types
    enum class OperatingMode { UNKNOWN, LOCAL, REMOTE };
    OperatingMode operating_mode_;

    enum class Direction { UNKNOWN, FORWARD, REVERSE };
    Direction direction_;

    enum class Motion { UNKNOWN, STOPPED, MOVING };
    Motion motion_;

    double rate_;



    // Syringe methods
    void set_rate(double rate);
    void set_operating_mode(OperatingMode operating_mode);
    void set_motion(Motion motion);
    void set_direction(Direction direction);

public slots:
    void syringe_set_forward();
    void syringe_set_local();
    void syringe_set_remote();

    void syringe_set_stop();
    void syringe_set_reverse();
    void syringe_switch_direction();
    void syringe_run();
    void syringe_set_rate(QString rate);
    void syringe_get_rate();

private:



    SerialConnection serial_connection_;

    // Methods
    void update_state(std::string serial_return_buffer);

signals:
    // Operating mode
    void state_update_operating_mode_local();
    void state_update_operating_mode_remote();

    // Motion
    void state_update_motion_moving();
    void state_update_motion_stopped();

    // Direction
    void state_update_direction_forward();
    void state_update_direction_reverse();

    // Rate
    void state_update_rate(double rate);


};

#endif // SYRINGEMODEL_H
