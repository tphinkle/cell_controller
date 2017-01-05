// Standard library
#include <iostream>
#include <time.h>
#include <chrono>
#include <math.h>

// Windows
#include <windows.h>

// Program specific
#include "rpmodel.h"

// Qt
#include <QApplication>

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
    sampling_frequency_ = 250000; //250000;
    samples_per_channel_ = pow(2,9); //512
    sample_buffer_length_ = pow(2,9); //512

    sample_buffer_ = new float64[sample_buffer_length_];

    // Buffer things
    data_buffer_length_ = pow(2,17); //131072
    data_buffer_head_ = 0;
    time_buffer_.resize(data_buffer_length_);
    for(unsigned int i = 0; i < time_buffer_.size(); i++)
    {
        time_buffer_[i] = i;
    }
    data_buffer_.resize(data_buffer_length_);


    // Set up buffers for displaying the baseline. This is sort of hackish, but is necessary to have to make plotting the baseline easier.
    baseline_time_buffer_.resize(2);                                                // baseline time
    baseline_time_buffer_[0] = time_buffer_[0];
    baseline_time_buffer_[1] = time_buffer_[time_buffer_.size() - 1];
    baseline_mean_buffer_.resize(2);                                                // baseline mean
    baseline_lower_thresh_buffer_.resize(2);                                        // baseline lower thresh
    baseline_upper_thresh_buffer_.resize(2);                                        // baseline upper thresh


    // Parser things
    i_ = 0;
    baseline_length_ = 200;
    looking_for_event_start_ = true;
    looking_for_event_stop_ = false;


}

RPModel::~RPModel()
{
    delete [] sample_buffer_;
}




void RPModel::start_main_loop()
{

    //QApplication::processEvents();

    create_DAQ_task();


    sample_DAQ();


    update_buffer();


    update_baseline(0);

    i_ = i_ + baseline_length_;



    while(thread_controller_->run() == true)
    {
        if(thread_controller_->control_syringe() == true)
        {
            process_buffer();             // Process the data in the buffer.
        }

        sample_DAQ();               // Sample the DAQ for more data.

        update_buffer();            // Add the new data from the DAQ to the buffer.
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
        data_buffer_[(data_buffer_head_+i)%data_buffer_length_] = sample_buffer_[i];
    }

    // Increment the buffer head
    data_buffer_head_ = (data_buffer_head_ + sample_buffer_length_)%data_buffer_length_;
    return;
}

void RPModel::process_buffer()
{
    // Control method: Everytime i is incremented, do one of two things
    // 1: i not caught up to buffer head -> return to start of while
    // 2: i caught up to buffer head -> stop the loop



    while(true)
    {

        if(looking_for_event_start_)
        {
            //std::cout << "1" << std::endl;
            look_for_event_start();
        }


        else if(looking_for_event_stop_)
        {
            //std::cout << "2" << std::endl;
            look_for_event_stop();
        }

        if(i_ == data_buffer_head_)
        {
            //std::cout << "3" << std::endl;
            return;
        }

    }

    return;

}


void RPModel::look_for_event_start()
{



    if((data_buffer_[i_] > baseline_.upper_thresh_) || \
    (data_buffer_[i_] < baseline_.lower_thresh_))
    {

        std::cout << "Event start found! 0" << std::endl;

        // Update baseline to be sure point is still outside of range
        int index = i_ - baseline_length_;


        if(index < 0)
        {
            index = data_buffer_length_ - i_ - baseline_length_;
        }

        update_baseline(index);



        // Check again
        if((data_buffer_[i_] > baseline_.upper_thresh_) || \
                (data_buffer_[i_] < baseline_.lower_thresh_))
        {
            std::cout << "Event start found! 1" << std::endl;
            looking_for_event_start_ = false;
            looking_for_event_stop_ = true;

        }


    }


    increment_i();


    return;

}

void RPModel::look_for_event_stop()
{

    if((data_buffer_[i_] <= baseline_.upper_thresh_) & \
        (data_buffer_[i_] >= baseline_.lower_thresh_))
    {
        // Emit event stop found
        // Stop recording RP
        // Stop recording camera...?
        // Reverse syringe

        std::cout << "\tEvent stop found!" << std::endl;

        looking_for_event_start_ = true;
        looking_for_event_stop_ = false;
    }

    increment_i();

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
        std_dev += pow(data_buffer_[i] - mean, 2.);
    }
    std_dev = pow(std_dev/baseline_length_, 0.5);

    baseline_.update(mean, std_dev, thread_controller_->threshold_multiplier());


    // Update baseline buffers for plotting
    baseline_mean_buffer_[0] = baseline_.mean_;
    baseline_mean_buffer_[1] = baseline_.mean_;

    baseline_lower_thresh_buffer_[0] = baseline_.lower_thresh_;
    baseline_lower_thresh_buffer_[1] = baseline_.lower_thresh_;

    baseline_upper_thresh_buffer_[0] = baseline_.upper_thresh_;
    baseline_upper_thresh_buffer_[1] = baseline_.upper_thresh_;


    return;

}


// Bit-wise operator to inrement the buffer reader (i_) that is faster than using modular arithmetic. Only works if buffer is of size 2^n for uint n.
void RPModel::increment_i()
{
    //i_ = (i_+1)&data_buffer_length_; // bit-wise
    i_ = (i_+1)%data_buffer_length_;

    return;
}
