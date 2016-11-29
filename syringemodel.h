#ifndef SYRINGEMODEL_H
#define SYRINGEMODEL_H

// Standard library
#include <string>

// Project specific
#include <serialconnection.h>

class SyringeModel
{
public:
    SyringeModel();


    // Syringe methods
    void set_local();
    void set_remote();
    void set_forward();
    void set_stop();
    void set_reverse();
    void switch_direction();
    void run();
    void set_rate(std::string rate);

private:


    enum Direction {  NONE, FORWARD, REVERSE };
    Direction direction_;

    enum Motion { STOPPED, MOVING };
    Motion motion_;

    SerialConnection serial_connection_;

    // Methods
    void update_state(std::string serial_return_buffer);

};

#endif // SYRINGEMODEL_H
