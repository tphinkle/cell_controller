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
    camera_buffer_size_ = 2048000000; // B (camera) total, divided between all cines
    recording_buffer_size_ = 500000000; // B (computer)
    camera_ip_ = "100.100.37.9";
    server_ip_ = "100.100.100.1";


    // Initialize
    tcplayer_.initialize(command_port_, data_port_, camera_ip_, server_ip_);

    // Set camera default parameters
    // frame_rate_, exposure_time_, res_x_, res_y_, num_images_
    set_default_all_parameters(10000, 5, 640, 480, 3000);



    // Start the data stream
    start_data_stream();


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
    // Needed for int to string conversions
    std::stringstream ss;

    std::cout << "Start record" << std::endl;

    // Determine which cine is active
    int current_cine = get_live_cine_number();
    if(current_cine == 0)
    {
        current_cine = 1;
    }




    std::string current_cine_str = std::to_string(current_cine);

    // Trigger the active cine to save the data on board
    std::cout << tcplayer_.send_command("trig\r") << std::endl;


    // Save counter for file names
    static int save_count = 0;
    // Initialize save file
    std::ofstream output_file("D:\\test_camera_"+std::to_string(save_count), std::ios::binary);
    save_count++;

    uchar* write_head = &recorded_image_buffer_[0];



    int start_index = get_cine_firstfr(current_cine);
    int stop_index = get_cine_lastfr(current_cine);
    std::cout << "stop_index " << stop_index << std::endl;
    std::cout << "start_index " << start_index << std::endl;
    int total_frames = stop_index - start_index+1;


    std::cout << total_frames << std::endl;
    std::cout << res_x_ << std::endl;
    std::cout << res_y_ << std::endl;
    std::cout << recording_buffer_size_ << std::endl;

    if((total_frames*res_x_*res_y_) >= recording_buffer_size_){
        std::cout << "?" << std::endl;
        // Num frames exceeds total buffer size; save in chunks instead.


        int j, k; // Left and right indices for saving.
        int interval; // This is the total number of frames to fetch per request.

        int total_saves = (total_frames*res_x_*res_y_)/(recording_buffer_size_) + 1;

        // Loop over chunks of data.
        for(int i = 0; i < total_saves; i++){
            std::cout << "Saving frame " << i + 1 << " out of " << total_saves << "." << std::endl;
            j = start_index + i*recording_buffer_size_/(res_x_*res_y_);  // Left point
            k = j + recording_buffer_size_/(res_x_*res_y_) - 1; // Right

            if(k > stop_index){  // This should only trigger on last iter; this is the left over stuff that doesn't fill the entire buffer
                k = stop_index;
            }



            interval = k-j;

            tcplayer_.send_data_request("img {cine:" + current_cine_str + ", start:"+std::to_string(j)+", cnt:"+std::to_string(interval)+"}\r",\
                                        recorded_image_buffer_,\
                                        (interval-3)*res_x_*res_y_); // -3 to prevent infinite loop; shouldn't really be here



            write_head = &recorded_image_buffer_[0];
            output_file.write((char*)write_head, (interval-3)*res_x_*res_y_);

        }

    }




    else{
        std::cout << "Shouldn't see this..." << std::endl;
        std::cout << "Saving " << total_frames << " images. Start: " << start_index << "." << std::endl;
        // Number of frames fits inside the buffer; call 'tcplayer_.send_data_request' just once.

        tcplayer_.send_data_request("img {cine:" + current_cine_str + ", start:"+std::to_string(start_index)+", cnt:"+std::to_string(total_frames)+"}\r",\
                                    recorded_image_buffer_, (total_frames-3)*res_x_*res_y_); // - 3 is to prevent infinite loop.

        output_file.write((char*)write_head, (total_frames-3)*res_x_*res_y_);
    }

    output_file.close(); // Close file

    std::cout << "Save finished!" << std::endl;

    return;


}

int CameraModel::get_cine_firstfr(int cine_num){
    // Return the index of the first frame stored in a cine
    std::string raw_result = tcplayer_.send_command("get c" + std::to_string(cine_num) + ".firstfr\r");


    raw_result.erase(remove_if(raw_result.begin(), raw_result.end(), isspace), raw_result.end());
    int str_length = raw_result.length();
    std::string result = raw_result.substr(8,str_length-8);



    return stoi(result);
}

int CameraModel::get_cine_lastfr(int cine_num){
    // Return the index of the last frame stored in a cine
    std::string raw_result =  tcplayer_.send_command("get c" + std::to_string(cine_num) + ".lastfr\r");

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
    std::cout << tcplayer_.send_command("get info.maxcines\r") << std::endl;
    for(int i = 0; i < 5; i++)
    {
        std::string cine_string = "get c" + std::to_string(i) + ".state\r";
        std::cout << tcplayer_.send_command(cine_string) << std::endl;
    }

    std::cout << tcplayer_.send_command("get info.*\r") << std::endl;

    //tcplayer_.send_command("get info.cinemem\r");

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
    int allocation_space = 0;
    if(image_size_*num_images_ > recording_buffer_size_){
        allocation_space = recording_buffer_size_;
    }
    else{
        allocation_space = image_size_*num_images_;
    }
    recorded_image_buffer_.resize(allocation_space);


    // Send the parameter info to the camera
    set_default_all_parameters_camera();


    return;
}


void CameraModel::set_default_all_parameters_camera(){

    // Allocate space on the camera cines
    std::cout << "Allocating space " << tcplayer_.send_command("alloc {"+ std::to_string(image_size_*num_images_/1000000.) + "}\r") << std::endl;


    // Set defaults for camera
    std::string cmd = "set defc.* {";
    cmd += "res:" + std::to_string(res_x_) + "x" + std::to_string(res_y_)+", ";
    cmd += "rate:" + std::to_string(frame_rate_)+", ";
    cmd += "ptframes:0, ";
    cmd += "exp:" + std::to_string(exposure_time_*1000)+"}\r";      // *1000: Convert to nanoseconds

    tcplayer_.send_command(cmd);

    tcplayer_.send_command("rec 1\r");

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

