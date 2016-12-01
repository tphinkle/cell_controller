// Standard library
#include <iostream>
#include <time.h>
#include <chrono>

// Windows
#include <windows.h>

// Program specific
#include "rpmodel.h"



RPModel::RPModel()
{
    // DAQ things
    sampling_frequency_ = 20000;
    samples_per_channel_ = 50000;
    array_size_ = 500;

    data_ref_ = new float64[array_size_];

    // Buffer things
    buffer_size_ = 1000000;
    buffer_head_ = 0;
    time_buffer_.resize(buffer_size_);
    for(unsigned int i = 0; i < time_buffer_.size(); i++)
    {
        time_buffer_[i] = i;
    }
    data_buffer_.resize(buffer_size_);



    // Plot things
    decimation_factor_ = 10;
    frames_per_sec_ = 10;

    frame_period_ms_ = static_cast<unsigned int>(1000./frames_per_sec_);
}


void RPModel::start_main_loop()
{
    create_DAQ_task();

    int i = 0;

    last_plot_update_time_ = get_time_ms();

    while(thread_controller_->run() == true)
    {
        if(i%10000000 == 0)
        {
            std::cout << "i = " << i << std::endl;
            if(thread_controller_->control_syringe() == true)
            {
                std::cout << "Syringe control is on " << std::endl;
                emit request_syringe_switch_direction();
            }


        }
        i++;
        sample_DAQ();
        update_buffer();
        parse_buffer();
        check_update_plot();
    }

    return;

}



void RPModel::create_DAQ_task()
{

    // Create the task
    DAQmxCreateTask("RP", &daq_task_handle_);


    // Create analog input voltage channel
    DAQmxCreateAIVoltageChan(&daq_task_handle_, "Dev1/ai0", "Voltage", DAQmx_Val_Cfg_Default, -10.0, 10.0, \
                             DAQmx_Val_Volts, NULL);

    // Configure timing parameters
    DAQmxCfgSampClkTiming(&daq_task_handle_, "OnboardClock", sampling_frequency_, DAQmx_Val_Rising,\
                     DAQmx_Val_ContSamps, samples_per_channel_);




    // Start the task. **This may belong in RPModel::start_DAQ_Task()!**
    DAQmxStartTask(&daq_task_handle_);
    return;
}


void RPModel::sample_DAQ()
{
    int32 num_read = 0;

    // TaskHandle taskHandle
    // int32 numSampsPerChan
    // float64 timeout
    // bool32 fillMode
    // float64 readArray[]
    // uInt32 arraySizeInSamps
    // int32 *sampsPerChanRead
    // bool32 *reserved

    DAQmxReadAnalogF64(&daq_task_handle_,\
                       array_size_,\
                       10.0,\
                       DAQmx_Val_GroupByScanNumber,\
                       data_ref_,\
                       array_size_,\
                       &num_read,\
                       NULL);

    if(num_read != 0)
    {
        std::cout << num_read << std::endl;
    }

    return;
}

void RPModel::update_buffer()
{

    return;
}

void RPModel::parse_buffer()
{

    return;
}

void RPModel::check_update_plot()
{



    unsigned int current_time = get_time_ms();

    if(current_time - last_plot_update_time_ > frame_period_ms_)
    {
        last_plot_update_time_ = get_time_ms();
        emit request_update_plot();
    }

    return;

}

unsigned int RPModel::get_time_ms()
{
    SYSTEMTIME t;
    GetSystemTime(&t);
    unsigned int time_ms = t.wHour*60*60*1000+\
            t.wMinute*60*1000+\
            t.wSecond*1000+\
            t.wMilliseconds;

    return time_ms;

}
