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
    sampling_frequency_ = 250000;//50000;
    samples_per_channel_ = 500;//50;
    sample_buffer_size_ = 500;

    sample_buffer_ = new float64[sample_buffer_size_];

    // Buffer things
    buffer_size_ = 100000;
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



    last_plot_update_time_ = get_time_ms();

    while(thread_controller_->run() == true)
    {
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
    DAQmxCreateTask("", &daq_task_handle_);


    // Create analog input voltage channel
    DAQmxCreateAIVoltageChan(daq_task_handle_, "Dev1/ai0", "", DAQmx_Val_Cfg_Default, -10.0, 10.0, \
                             DAQmx_Val_Volts, NULL);

    // Configure timing parameters
    DAQmxCfgSampClkTiming(daq_task_handle_, "OnboardClock", sampling_frequency_, DAQmx_Val_Rising,\
                     DAQmx_Val_ContSamps, samples_per_channel_);




    // Start the task. **This may belong in RPModel::start_DAQ_Task()!**
    DAQmxStartTask(daq_task_handle_);

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

    DAQmxReadAnalogF64(daq_task_handle_,\
                       sample_buffer_size_,\
                       10.0,\
                       DAQmx_Val_GroupByScanNumber,\
                       sample_buffer_,\
                       sample_buffer_size_,\
                       &num_read,\
                       NULL);



    return;
}

void RPModel::update_buffer()
{
    // Shift sample reference to buffer
    for(unsigned int i = 0; i < sample_buffer_size_; i++)
    {
        data_buffer_[(buffer_head_+i)%buffer_size_] = sample_buffer_[i];
    }

    // Increment the buffer head
    buffer_head_ = (buffer_head_ + sample_buffer_size_)%buffer_size_;
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
        // Update the plot
        emit request_update_plot();
        last_plot_update_time_ = get_time_ms();
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
