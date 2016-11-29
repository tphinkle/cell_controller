// Program specific
#include "rpmodel.h"

// Standard library
#include <iostream>

RPModel::RPModel()
{
    sampling_frequency_ = 20000;
    samples_per_channel_ = 50000;


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

void RPModel::start_DAQ_task()
{

    return;
}
