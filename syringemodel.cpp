#include "syringemodel.h"

#include <iostream>
#include <string>


SyringeModel::SyringeModel()
{

    operating_mode_ = OperatingMode::UNKNOWN;
    motion_ = Motion::UNKNOWN;
    direction_ = Direction::UNKNOWN;

}

void SyringeModel::syringe_set_local()
{
    serial_connection_.write("KEY");

    return;
}

void SyringeModel::syringe_set_remote()
{
    std::string result = serial_connection_.write("\r");

    update_state(result);

    return;
}

void SyringeModel::syringe_set_forward()
{
    if(direction_ == Direction::UNKNOWN)
    {
        syringe_set_remote();
    }
    else if(direction_ == Direction::REVERSE)
    {
        syringe_switch_direction();
    }

    if(motion_ == Motion::STOPPED)
    {
        syringe_run();
    }

    return;
}

void SyringeModel::syringe_set_stop()
{
    std::string result = serial_connection_.write("STP\r");

    update_state(result);
    return;
}

void SyringeModel::syringe_set_reverse()
{
    if(direction_ == Direction::UNKNOWN)
    {
        syringe_set_remote();
    }
    else if(direction_ == Direction::FORWARD)
    {
        syringe_switch_direction();
    }
    if( motion_ == Motion::STOPPED)
    {
        syringe_run();
    }

    return;
}

void SyringeModel::syringe_switch_direction()
{
    std::string result = serial_connection_.write("REV\r");

    update_state(result);
    return;
}

void SyringeModel::syringe_run()
{
    std::string result = serial_connection_.write("RUN\r");

    update_state(result);
    return;
}

void SyringeModel::syringe_get_rate()
{
    std::cout << "Setting rate in syringe 2. " << std::endl;
    std::string result = serial_connection_.write_data_request("RAT\r");
    set_rate(std::stod(result));

    return;
}

void SyringeModel::syringe_set_rate(QString rate)
{
    std::string result = serial_connection_.write("MLM"+rate.toStdString()+"\r");

    syringe_get_rate();

    return;
}

void SyringeModel::update_state(std::string serial_return_buffer)
{



    set_operating_mode(OperatingMode::REMOTE);

    if (serial_return_buffer == ":")
    {

        set_motion(Motion::STOPPED);
    }
    else
    {
        set_motion(Motion::MOVING);


        if( serial_return_buffer == ">")
        {
            set_direction(Direction::FORWARD);
        }

        else if( serial_return_buffer == "<")
        {            
            set_direction(Direction::REVERSE);
        }

    }

    return;
}

void SyringeModel::set_operating_mode(OperatingMode operating_mode)
{
    operating_mode_ = operating_mode;
    if(operating_mode_ == OperatingMode::LOCAL)
    {
        emit(state_update_operating_mode_local());
    }
    else if(operating_mode_ == OperatingMode::REMOTE)
    {
        emit(state_update_operating_mode_remote());
    }

    return;
}

void SyringeModel::set_motion(Motion motion)
{
    motion_ = motion;
    if(motion_ == Motion::MOVING)
    {
        emit(state_update_motion_moving());
    }
    else if(motion_ == Motion::STOPPED)
    {
        emit(state_update_motion_stopped());
    }

    return;
}

void SyringeModel::set_direction(Direction direction)
{
    direction_ = direction;
    if(direction_ == Direction::FORWARD)
    {
        emit(state_update_direction_forward());
    }
    else if(direction_ == Direction::REVERSE)
    {
        emit(state_update_direction_reverse());
    }

    return;
}

void SyringeModel::set_rate(double rate)
{
    std::cout << "Setting rate in syringe." << std::endl;
    rate_ = rate;
    emit(state_update_rate(rate));

    return;
}
