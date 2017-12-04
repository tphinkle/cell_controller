// Standard library
#include <stdlib.h>
#include <iostream>
#include <time.h>
#include <chrono>
#include <math.h>
#include <fstream>
#include <iterator>
#include <sstream>

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
    data_buffer_length_ = pow(2,20); //131072
    data_buffer_head_ = 0;
    time_buffer_.resize(data_buffer_length_);
    for(unsigned int i = 0; i < time_buffer_.size(); i++)
    {
        time_buffer_[i] = i;
    }
    data_buffer_.resize(data_buffer_length_);
    std::fill(data_buffer_.begin(), data_buffer_.end(), 0);

    // Set up plot buffers
    decimation_factor_ = 64;
    plot_data_buffer_length_ = data_buffer_length_/decimation_factor_;
    plot_time_buffer_ = new double[plot_data_buffer_length_];
    plot_data_buffer_ = new double[plot_data_buffer_length_];


    // Set the memory location of the plot buffers
    for(int i = 0; i < plot_data_buffer_length_; i++){
        plot_time_buffer_[i] = time_buffer_[i*decimation_factor_];
    }


    // Set up buffers for displaying the baseline. This is sort of hackish, but is necessary to have to make plotting the baseline easier.
    baseline_time_buffer_.resize(2);                                                // baseline time
    baseline_time_buffer_[0] = time_buffer_[0];
    baseline_time_buffer_[1] = time_buffer_[time_buffer_.size() - 1];
    baseline_mean_buffer_.resize(2);                                                // baseline mean
    baseline_lower_thresh_buffer_.resize(2);                                        // baseline lower thresh
    baseline_upper_thresh_buffer_.resize(2);                                        // baseline upper thresh


    // Parser things
    i_ = 0;
    baseline_length_ = 500;
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
            std::cout << "Processing buffer" << std::endl;
            process_buffer();             // Process the data in the buffer.
        }



        sample_DAQ();               // Sample the DAQ for more data.

        update_buffer();            // Add the new data from the DAQ to the buffer.

        save_buffer();
    }



    return;

}

void RPModel::save_buffer()
{
    if(thread_controller_->save_buffer() == true)
    {
        static int save_index = 0;
        std::stringstream save_index_ss;
        save_index_ss << save_index;
        std::string save_index_string = save_index_ss.str();
        std::ofstream output_file("E:\\test_rp_" + save_index_string, std::ios::binary);
        save_index++;
        for(int i = 0; i < data_buffer_.size(); i++)
        {
            output_file << data_buffer_[(data_buffer_head_ + i + 1)%data_buffer_length_];
            output_file << "\n";
        }


        thread_controller_->set_save_buffer(false);
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

    //static int total_points = 0;
    //static std::chrono::milliseconds start_time = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch());


    // Shift sample reference to buffer
    for(unsigned int i = 0; i < sample_buffer_length_; i++)
    {
        data_buffer_[(data_buffer_head_+i)%data_buffer_length_] = sample_buffer_[i];
    }

    // Update the plot buffer
    for(unsigned int i = 0; i < sample_buffer_length_/decimation_factor_; i++){
        plot_data_buffer_[(data_buffer_head_/decimation_factor_ + i)%plot_data_buffer_length_] = data_buffer_[(data_buffer_head_+i*decimation_factor_)%data_buffer_length_];
    }

    // Increment the buffer head
    data_buffer_head_ = (data_buffer_head_ + sample_buffer_length_)%data_buffer_length_;


    //total_points += sample_buffer_length_;
    //std::chrono::milliseconds current_time = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch());

    //std::cout << "Total points: " << total_points << std::endl;
    //std::cout << "delta time: " << current_time.count() - start_time.count() << std::endl;






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
            event_start_found();
            return;


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
        event_stop_found();
        return;
    }

    increment_i();

    return;
}

void RPModel::event_start_found()
{
    if(thread_controller_->control_syringe())
    {
        emit request_syringe_switch_direction();
    }

    if(thread_controller_->control_camera())
    {
        emit request_camera_record();
    }

    looking_for_event_start_ = false;
    looking_for_event_stop_ = true;
    return;
}

void RPModel::event_stop_found()
{
    //emit reverse_syringe();
    looking_for_event_start_ = true;
    looking_for_event_stop_ = false;
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
