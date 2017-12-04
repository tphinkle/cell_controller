#include "cameramodel.h"



// Standard library
#include <stdlib.h>
#include <iostream>
#include <fstream>
#include <string>
#include <chrono>
#include <unistd.h>
#include <sstream>
#include <regex> // For string parsing.

// Windows
#include <windows.h>

CameraModel::CameraModel()
{

    // Constants
    command_port_ = 7115;
    data_port_ = 15234;

    //camera_buffer_size_ = 2048000000; // B (camera) total, divided between all cines       (v7.1
    camera_buffer_size_ = 40000000000; // B (camera) total, divided between all cines    (v2511)

    recording_buffer_size_ = 500000000; // B (computer)
    //camera_ip_ = "100.100.37.9";    // V7.1
    camera_ip_ = "100.100.125.66";     // V2511
    server_ip_ = "100.100.100.1";

    //PHANTOM_SERIES series = PHANTOM_SERIES::V7;   // High-speed
    PHANTOM_SERIES series = PHANTOM_SERIES::V16;    // Ultra high-speed


    // Initialize
    tcplayer_.initialize(command_port_, data_port_, camera_ip_, server_ip_);

    // Set camera default parameters
    // frame_rate_, exposure_time_, res_x_, res_y_, num_images_
    std::cout << "Setting default parameters to camera." << std::endl;
    // set_default_all_parameters(15000, 20, 512, 288, 13500);    // For v7
    //set_default_all_parameters(5000, 20, 384, 288, 50);    // V16

    //set_default_all_parameters(100000, 2, 512, 288, 50);    // V16
    set_default_all_parameters(250000, 1, 384, 112, 250);    // V16 100k fps


    std::cout << "Starting data stream." << std::endl;

    // Start the data stream
    start_data_stream();

    std::cout << "Starting recording." << std::endl;

    tcplayer_.send_command("rec 1\r");




}



CameraModel::~CameraModel()
{

}

void CameraModel::start_data_stream()
{
    std::cout << "Starting data stream." << std::endl;
    std::string command = "startdata {port:" + std::to_string(data_port_) + "}\r";

    tcplayer_.send_command(command);
    return;
}

void CameraModel::get_live_image()
{
    std::string req = "img {cine:-1, start:-1, cnt:1}\r";
    tcplayer_.send_data_request(req, live_image_buffer_);

    emit(state_update_live_image(res_x_, res_y_));

    return;
}

int CameraModel::get_live_cine_number()
{


    std::string result = tcplayer_.send_command("cstats\r");

    std::vector<std::string> string_vec = split_string(result, "\n");

    for(int i = 0; i < string_vec.size(); i++)
    {
        if(string_vec[i].find("ACT") != std::string::npos) // 'ACT' flag found!
        {
            return i;
        }
    }

    return -1; // Couldn't get live cine...
}

void CameraModel::record()
{
    std::cout << "Start record" << std::endl;


    // Determine which cine is active
    int current_cine = get_live_cine_number();
    if(current_cine == 0)    // 0 is the preview cine; if 0, set to 1.
    {
        current_cine = 1;
    }
    std::string current_cine_str = std::to_string(current_cine);


    // Trigger the active cine to save the data on board
    std::cout << tcplayer_.send_command("trig\r") << std::endl;



    Sleep(1000);    // Sleep put in to make sure the trigger is received and acted on before calls to retrieve the data are made
                    // The camera will sometimes bug out on large recordings w/o this sleep call.
                    // Units = ms.


    // Save counter for file names; increments by 1 every save
    static int save_count = 0;
    // Initialize save file
    std::ofstream output_file("E:\\test_camera_"+std::to_string(save_count), std::ios::binary);
    save_count++;



    uchar* write_head = &recorded_image_buffer_[0];

    long start_index = get_cine_firstfr(current_cine);

    //start_index = -50000;    // -50000 is hack for V16 camera; remove this line for V7
    start_index = -500000;    // 100000 FPS change -50000 is hack for V16 camera; remove this line for V7
    //start_index = -5000;
    int stop_index = get_cine_lastfr(current_cine);
    std::cout << "stop_index " << stop_index << std::endl;
    std::cout << "start_index " << start_index << std::endl;
    long total_frames = stop_index - start_index + 1;
    long total_size = total_frames*res_x_*res_y_;



    std::cout << "Total frames: " << total_frames << std::endl;
    std::cout << "Resolution x: " << res_x_ << std::endl;
    std::cout << "Resolution y: " << res_y_ << std::endl;
    std::cout << "Recording buffer size: " << recording_buffer_size_ << std::endl;
    std::cout << "Total size: " << total_size << std::endl;

    //if((total_size) >= (long)(recording_buffer_size_))
    if(1 > 0){
        // Num frames exceeds total buffer size; save in chunks.

        int j, k; // Left and right indices for saving.
        int interval; // This is the total number of frames to fetch per request.

        int total_saves = (1.*total_frames*res_x_*res_y_)/(recording_buffer_size_) + 1;

        int total_bytes;   // Bytes to request, save for each chunk

        std::cout << "Total saves = " << total_saves << std::endl;

        // Loop over chunks of data.
        for(int i = 0; i < total_saves; i++){
            std::cout << "\tSaving frame " << i + 1 << " out of " << total_saves << "." << std::endl;
            j = start_index + 1.*i*recording_buffer_size_/(res_x_*res_y_);  // Left point
            k = j + recording_buffer_size_/(res_x_*res_y_) - 1; // Right

            if(k > stop_index){  // This should only trigger on last iter; this is the left over stuff that doesn't fill the entire buffer
                k = stop_index;
            }

            interval = k-j;

            total_bytes = interval*res_x_*res_y_;

            std::cout << "\t\tstart: " << j << "\tstop: " << k << std::endl;


            // Get the data
            // Args:
            //  - String command to camera
            //  - Buffer to be filled
            //  - Number of bytes to request
            tcplayer_.send_data_request("img {cine:" + current_cine_str + ", start:"+std::to_string(j)+", cnt:"+std::to_string(interval)+"}\r",\
                                        recorded_image_buffer_,\
                                        total_bytes);    // was (interval-3)*res_x_*res_y_
                                                                        // was -3
                                                                        // -3 to prevent infinite loop; shouldn't really be here


            // Set the write head to the beginning of the recording buffer and write to file.
            write_head = &recorded_image_buffer_[0];
            output_file.write((char*)write_head, total_bytes);

        }

    }




    else{
        // Number of frames fits inside the buffer; call 'tcplayer_.send_data_request' just once.

        std::cout << "Saving " << total_frames << " images. Start: " << start_index << "." << std::endl;
        return;
        tcplayer_.send_data_request("img {cine:" + current_cine_str + ", start:"+std::to_string(start_index)+", cnt:"+std::to_string(total_frames)+"}\r",\
                                    recorded_image_buffer_, (total_frames-3)*res_x_*res_y_); // - 3 is to prevent infinite loop.

        output_file.write((char*)write_head, (total_frames-3)*res_x_*res_y_);
    }



    // Tidy up.
    std::cout << "Save finished!" << std::endl;
    output_file.close();
    tcplayer_.send_command("rec 1\r");    // Resume recording in first cine.

    return;


}

int CameraModel::get_cine_firstfr(int cine_num){
    // Return the index of the first frame stored in a cine
    cine_num = 1;
    std::string raw_result = tcplayer_.send_command("get c" + std::to_string(cine_num) + ".firstfr\r");

    std::cout << "First frame raw result: " << raw_result << std::endl;

    raw_result.erase(remove_if(raw_result.begin(), raw_result.end(), isspace), raw_result.end());
    int str_length = raw_result.length();
    std::string result = raw_result.substr(8,str_length-8);



    return stoi(result);
}

int CameraModel::get_cine_lastfr(int cine_num){
    // Return the index of the last frame stored in a cine
    cine_num = 1;
    std::string raw_result =  tcplayer_.send_command("get c" + std::to_string(cine_num) + ".lastfr\r");

    std::cout << "Last frame raw result: " << raw_result << std::endl;

    raw_result.erase(remove_if(raw_result.begin(), raw_result.end(), isspace), raw_result.end());
    int str_length = raw_result.length();
    std::string result = raw_result.substr(7,str_length-7);

    return stoi(result);
}

int CameraModel::get_cine_frcount(int cine_num)
{
    // Return the index of the last frame stored in a cine
    std::string raw_result =  tcplayer_.send_command("get c" + std::to_string(cine_num) + ".frcount\r");

    raw_result.erase(remove_if(raw_result.begin(), raw_result.end(), isspace), raw_result.end());
    int str_length = raw_result.length();
    std::string result = raw_result.substr(8,str_length-8);


    return stoi(result);
}

void CameraModel::get_cine_info()
{
    // std::cout << tcplayer_.send_command("get info.maxcines\r") << std::endl;
    std::cout << tcplayer_.send_command("get c1.frspace\r") << std::endl;
    std::cout << tcplayer_.send_command("get c1.frsize\r") << std::endl;
    std::cout << tcplayer_.send_command("get c1.res\r") << std::endl;
    std::cout << tcplayer_.send_command("get c1.ptframes\r") << std::endl;
    std::cout << tcplayer_.send_command("get c1.rate\r") << std::endl;
    std::cout << tcplayer_.send_command("get c1.exp\r") << std::endl;
    std::cout << "xinc and yinc" << std::endl;
    std::cout << tcplayer_.send_command("get info.xinc\r") << std::endl;
    std::cout << tcplayer_.send_command("get info.yinc\r") << std::endl;
    for(int i = 0; i < 2; i++)
    {
        std::string cine_string = "get c" + std::to_string(i) + ".state\r";
        std::cout << tcplayer_.send_command(cine_string) << std::endl;
    }

    return;
}


void CameraModel::set_default_all_parameters(int frame_rate, int exposure_time, int res_x, int res_y, int num_images)
{


    // Check parameter validity
    if(1./frame_rate < 0.000001*exposure_time){
        std::cout << "Invalid frame rate and exposure time entries!" << std::endl;
        return;
    }

    if(num_images*res_x*res_y > camera_buffer_size_){
        std::cout << "Could not allocate memory! Max on-board camera memory is 2048 MB." << std::endl;
        return;
    }



    // Parameters are valid, continue.

    // Frame rate, exposure time
    frame_rate_ = frame_rate;
    exposure_time_ = exposure_time;


    // Resolution
    res_x_ = res_x;
    res_y_ = res_y;
    image_size_ = res_x_*res_y_;

    // Number of images to store per cine
    num_images_ = num_images;

    // Number of cines
    num_cines_ = 1;

    // Reallocate memory in live image buffer
    live_image_buffer_.resize(res_x_*res_y_);      // Buffer needs to be resized.

    live_image_pointer_ = &live_image_buffer_[0];  // Buffer head needs to be put back to beginning.




    // Reallocate memory in recorded image buffer


    //////////////
    // V7!
    //////////////
    int allocation_space = 0;
    //num_images_ = frame_rate_*0.5;    // V16  // This means store 0.5 seconds worth of footage

    std::cout << "image size: " << image_size_ << std::endl;
    std::cout << "num images: " << num_images_ << std::endl;
    std::cout << "recording buffer size " << recording_buffer_size_ << std::endl;

    if(image_size_*num_images_ > recording_buffer_size_){
        allocation_space = recording_buffer_size_;

    }
    else{
        allocation_space = image_size_*num_images_;

    }



    //////////////
    // V16!
    //////////////
    allocation_space = recording_buffer_size_;    // V16!

    recorded_image_buffer_.resize(allocation_space);


    // Send the parameter info to the camera
    set_default_all_parameters_camera();


    return;
}


void CameraModel::set_default_all_parameters_camera(){

    // Allocate space on the camera cines... V7.1 only!!!!
    //std::cout << "Allocating space " << tcplayer_.send_command("alloc {"+ std::to_string(image_size_*num_images_/1000000.) + "}\r") << std::endl;


    // Set defaults for camera
    std::string cmd = "set defc.* {";
    //std::string cmd = "set c1.* {";
    cmd += "res:" + std::to_string(res_x_) + "x" + std::to_string(res_y_)+", ";
    cmd += "rate:" + std::to_string(frame_rate_)+", ";
    cmd += "ptframes:0, ";
    cmd += "frspace:" + std::to_string(880*num_images_) +", ";    // V16; reserve memory; 2416 is the image size (in kernels) per image for 640x480; 880 for 384x288;

    //std::cout << "SETTING EXPOSURE MANUALLY TO 500 NS!" << std::to_string(exposure_time_*1000) << std::endl;

    cmd += "exp:" + std::to_string(exposure_time_*1000)+"}\r";      // *1000: Convert to nanoseconds
    //cmd += "exp:" + std::to_string(1000)+"}\r";      // *1000: Convert to nanoseconds


    std::cout << "setting status:" << tcplayer_.send_command(cmd) << std::endl;


    return;
}

int CameraModel::frame_rate(){
    return frame_rate_;
}

int CameraModel::num_images(){
    return num_images_;
}


std::vector<std::string> CameraModel::split_string(std::string str, std::string split_str)
{
    std::vector<std::string> tokens;
    std::size_t start = 0, end = 0;

    while((end = str.find(split_str, start)) != std::string::npos)
    {
        if(end != start)
        {
            tokens.push_back(str.substr(start, end-start));
        }

        start = end + 1;
    }

    if (end != start)
    {
        tokens.push_back(str.substr(start));

    }



    return tokens;

}

