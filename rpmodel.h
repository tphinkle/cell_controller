#ifndef RPMODEL_H
#define RPMODEL_H


// Standard Library
#include <vector>

// DAQmx
// Note that the first **must** be included when importing NIDAQmx header due to mingw
// compiler not detecting Microsoft's __int64 type natively.
// See the following thread:
// forums.ni.com/t5/Multifunction-DAQ/Howto-use-NIDAQmx-with-mingw-gcc-3-4-2/m-p/294361?jump=true
#include <stdio.h>
#include <C:\Program Files\National Instruments\NI-DAQ\DAQmx ANSI C Dev\include\NIDAQmx.h>

// Qt
#include <QThread>

// Project related
#include <RPThreadController.h>

class Baseline
{
public:
    Baseline();

    double mean_;
    double lower_thresh_;
    double upper_thresh_;
    void update(double mean, double std_dev, double multiplier);


};

class RPModel : public QObject
{

    Q_OBJECT

public:
    RPModel();
    ~RPModel();

    // Loop control
    RPThreadController* thread_controller_;


    // Buffer things
    int i_;
    unsigned int data_buffer_length_;
    unsigned int data_buffer_head_;
    std::vector<double> time_buffer_;
    std::vector<double> data_buffer_;
    double* plot_time_buffer_;
    double* plot_data_buffer_;
    int plot_data_buffer_length_;

    // For plotting the mean, upper thresh, and lower thresh lines
    std::vector<double> baseline_time_buffer_;
    std::vector<double> baseline_mean_buffer_;
    std::vector<double> baseline_lower_thresh_buffer_;
    std::vector<double> baseline_upper_thresh_buffer_;


    // Processor things
    Baseline baseline_;
    int baseline_length_;
    bool looking_for_event_stop_;
    bool looking_for_event_start_;






signals:
    // For communicating with other threads.
    void request_syringe_switch_direction();
    void request_camera_record();

public slots:
    void start_main_loop();



private:
    ///////////////////////////////////////////
    // Functions
    ///////////////////////////////////////////

    // DAQmx things
    void create_DAQ_task();
    void sample_DAQ();

    // Buffer
    void update_buffer();
    void save_buffer();

    // Processer
    void process_buffer();
    void update_baseline(int index);
    void look_for_event_start();
    void look_for_event_stop();
    void increment_i();
    void event_start_found();
    void event_stop_found();



    ///////////////////////////////////////////
    // Objects
    ///////////////////////////////////////////



    // DAQmx things
    TaskHandle daq_task_handle_;
    int32 sampling_frequency_;      // Total sample rate =
    int32 samples_per_channel_;     //       sampling_frequency_ x samples_per_channel_
    uInt32 sample_buffer_length_;
    float64* sample_buffer_;


    // Parser things



    // Plot things
    unsigned int decimation_factor_;
    unsigned int frames_per_sec_;
    unsigned int frame_period_ms_;
    unsigned int last_plot_update_time_;


};

#endif // RPMODEL_H
