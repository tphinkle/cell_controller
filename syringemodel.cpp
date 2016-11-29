#include "syringemodel.h"

#include <iostream>
#include <string>


SyringeModel::SyringeModel()
{

    direction_ = NONE;
}


void SyringeModel::set_local()
{
    serial_connection_.write("KEY");

    return;
}

void SyringeModel::set_remote()
{
    std::string result = serial_connection_.write("\r");

    update_state(result);

    return;
}

void SyringeModel::set_forward()
{
    if(direction_ == REVERSE)
    {
        switch_direction();
    }

    run();

    return;
}

void SyringeModel::set_stop()
{
    std::string result = serial_connection_.write("STP\r");

    update_state(result);
    return;
}

void SyringeModel::set_reverse()
{
    if(direction_ == FORWARD)
    {
        switch_direction();
    }

    run();

    return;
}

void SyringeModel::switch_direction()
{
    std::string result = serial_connection_.write("REV\r");

    update_state(result);
    return;
}

void SyringeModel::run()
{
    std::string result = serial_connection_.write("RUN\r");

    update_state(result);
    return;
}

void SyringeModel::set_rate(std::string rate)
{
    std::cout << "strying to set rate..." << "MLM"+rate << std::endl;
    std::string result = serial_connection_.write("MLM"+rate+"\r");
    return;
}

void SyringeModel::update_state(std::string serial_return_buffer)
{

    //std::cout << "seral return buffer" << serial_return_buffer << std::endl;

    if (serial_return_buffer == ":")
    {
        //std::cout << "STOPPED" << std::endl;
        motion_ = STOPPED;
    }
    else
    {
        motion_ = MOVING;
        //std:: cout << "MOVING" << std::endl;

        if( serial_return_buffer == ">")
        {
            //std::cout << "FORWARD" << std::endl;
            direction_ = FORWARD;
        }

        else if( serial_return_buffer == "<")
        {
            //std::cout << "REVERSE" << std::endl;
            direction_ = REVERSE;
        }

    }

    return;
}
