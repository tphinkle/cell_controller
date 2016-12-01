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

class RPModel : public QObject
{

    Q_OBJECT

public:
    RPModel();

    // Loop control
    RPThreadController* thread_controller_;


    // Buffer things
    unsigned int buffer_size_;
    unsigned int buffer_head_;
    std::vector<double> time_buffer_;
    std::vector<double> data_buffer_;

signals:
    void request_update_plot();
    void request_syringe_switch_direction();

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

    // Parser
    void parse_buffer();

    // Plot
    void check_update_plot();

    // Misc.
    unsigned int get_time_ms();


    ///////////////////////////////////////////
    // Objects
    ///////////////////////////////////////////



    // DAQmx things
    TaskHandle daq_task_handle_;

    int32 sampling_frequency_;
    int32 samples_per_channel_;
    uInt32 array_size_;

    float64* data_ref_;        // Must be float64



    // Parser things



    // Plot things
    unsigned int decimation_factor_;
    unsigned int frames_per_sec_;
    unsigned int frame_period_ms_;
    unsigned int last_plot_update_time_;


};

#endif // RPMODEL_H
