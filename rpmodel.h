#ifndef RPMODEL_H
#define RPMODEL_H

// DAQmx
// Note that the first **must** be included when importing NIDAQmx header due to mingw
// compiler not detecting Microsoft's __int64 type natively.
// See the following thread:
// forums.ni.com/t5/Multifunction-DAQ/Howto-use-NIDAQmx-with-mingw-gcc-3-4-2/m-p/294361?jump=true
#include <stdio.h>
#include <C:\Program Files\National Instruments\NI-DAQ\DAQmx ANSI C Dev\include\NIDAQmx.h>


class RPModel : public QThread
{
public:
    RPModel();

    void create_DAQ_task();
    void start_DAQ_task();


private:
    TaskHandle daq_task_handle_;

    unsigned int sampling_frequency_;
    unsigned int samples_per_channel_;

};

#endif // RPMODEL_H
