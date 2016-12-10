// Standard library
#include <iostream>
#include <time.h>
#include <chrono>

// Windows
#include <windows.h>

// Program specific
#include "rpmodel.h"

Baseline::Baseline()
{



}

void Baseline::update(double mean, double std_dev, double multiplier)
{
    mean_ = mean;
    upper_thresh_ = mean + std_dev*multiplier;
    lower_thresh_ = mean - std_dev*multiplier;

    return;

}

RPModel::RPModel()
{


    // DAQ things
    sampling_frequency_ = 250000;//50000;
    samples_per_channel_ = 500;//50;
    sample_buffer_length_ = 500;

    sample_buffer_ = new float64[sample_buffer_length_];

    // Buffer things
    buffer_length_ = 100000;
    i_.buffer_length_ = buffer_length_;
    buffer_head_ = 0;
    time_buffer_.resize(buffer_length_);
    for(unsigned int i = 0; i < time_buffer_.size(); i++)
    {
        time_buffer_[i] = i;
    }
    data_buffer_.resize(buffer_length_);


    // Parser things
    looking_for_event_start_ = true;
    looking_for_event_stop_ = false;
    threshold_multiplier_ = 5;


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
        // check_update_plot();
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
                       sample_buffer_length_,\
                       10.0,\
                       DAQmx_Val_GroupByScanNumber,\
                       sample_buffer_,\
                       sample_buffer_length_,\
                       &num_read,\
                       NULL);



    return;
}

void RPModel::update_buffer()
{
    // Shift sample reference to buffer
    for(unsigned int i = 0; i < sample_buffer_length_; i++)
    {
        data_buffer_[(buffer_head_+i)%buffer_length_] = sample_buffer_[i];
    }

    // Increment the buffer head
    buffer_head_ = (buffer_head_ + sample_buffer_length_)%buffer_length_;
    return;
}

void RPModel::parse_buffer()
{
    // Control method: Everytime i is incremented, do one of two things
    // 1: i not caught up to buffer head -> return to start of while
    // 2: i caught up to buffer head -> stop the loop


    // LookForEventStart()

    // LookForEventStop()


    return;
}


void RPModel::look_for_event_start()
{
    while(looking_for_event_start_ == true)
    {
        if((data_buffer_[i_] > baseline_.upper_thresh_) || \
           (data_buffer_[i_] < baseline_.lower_thresh_))
        {
            update_baseline(i_ - baseline_length_);

        }

    }

    return;
}

void RPModel::look_for_event_stop()
{

    return;
}

void RPModel::update_baseline(int index)
{
    double mean = 0;

    for(int i = index; i < index + baseline_length_; i++)
    {
        mean += data_buffer_[i];
    }
    mean = mean/baseline_length_;

    double std_dev = 0;
    for(int i = index; i < index + baseline_length_; i++)
    {
        std_dev = std_dev + std::pow(data_buffer_[i] - mean, 2);
    }
    std_dev = std_dev/baseline_length_;

    baseline_.update(mean, std_dev, threshold_multiplier_);

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
