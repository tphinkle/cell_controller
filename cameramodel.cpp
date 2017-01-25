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
    max_memory_ = 2048; //MB (camera)
    max_buffer_size_ = 500; // MB (computer)
    camera_ip_ = "100.100.37.9";
    server_ip_ = "100.100.100.1";

    // Camera variables
    frame_rate_ = 10000; // fps
    exposure_time_ = 2000; // in ns
    num_images_ = 3000; // Number of images to request


    // Initialize
    tcplayer_.initialize(command_port_, data_port_, camera_ip_, server_ip_);

    // Set resolution
    set_resolution(640, 480);





    //set_live_parameters_camera();

    //start_data_stream();

}

CameraModel::set_resolution(int res_x, int res_y){
    // Set parameters
    res_x_ = res_x;
    res_y_ = res_y;
    image_size_ = res_x_*res_y_;

    // Resize live image buffer
    live_image_buffer_.resize(image_size_);
    live_image_pointer_ = &live_image_buffer_[0];

    // Resize recorded image buffer
    int reserve_size;
    if(image_size_*num_images_ > max_buffer_size_*1000000){
        reserve_size = max_buffer_size_*1000000;
    }
    else{
        reserve_size = image_size_*num_images_;
    }

    recorded_image_buffer_.resize(reserve_size);

    set_default_parameters();

    return;
}

CameraModel::~CameraModel()
{
    delete live_image_pointer_;
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
        if(string_vec[i].find("ACT") != std::string::npos)
        {
            return i;
        }
    }

    return -1;
}

void CameraModel::record()
{
    // Needed for int to string conversions
    std::stringstream ss;

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
    int total_frames = stop_index - start_index+1;


    int frcount = get_cine_frcount(current_cine);



    if((total_frames*res_x_*res_y_) >= max_buffer_size_*1000000){
        // Num frames exceeds total buffer size; save in chunks instead.


        int j, k; // Left and right indices for saving.
        int interval; // This is the total number of frames to fetch per request.



        // Loop over chunks of data.
        for(int i = 0; i < (total_frames*res_x_*res_y_)/(max_buffer_size_*1000000) + 1; i++){
            std::cout << "i = " << i << std::endl;
            j = start_index + i*max_buffer_size_*1000000/(res_x_*res_y_);  // Left point
            k = j + max_buffer_size_*1000000/(res_x_*res_y_) - 1; // Right

            if(k > stop_index){  // This should only trigger on last iter; this is the left over stuff that doesn't fill the entire buffer
                k = stop_index;
            }



            interval = k-j;

            tcplayer_.send_data_request("img {cine:" + current_cine_str + ", start:"+std::to_string(j)+", cnt:"+std::to_string(interval)+"}\r",\
                                        recorded_image_buffer_,\
                                        (interval-3)*res_x_*res_y_);



            write_head = &recorded_image_buffer_[0];

            output_file.write((char*)write_head, (interval-3)*res_x_*res_y_);

        }

    }




    else{
        std::cout << "start_index = " << start_index << std::endl;
        std::cout << "stop_index = " << stop_index << std::endl;
        std::cout << "total_frames = " << total_frames << std::endl;
        // Num frames fits inside the buffer; do a normal retrieve and write.
        tcplayer_.send_data_request("img {cine:" + current_cine_str + ", start:"+std::to_string(start_index)+", cnt:"+std::to_string(total_frames)+"}\r",\
                                    recorded_image_buffer_, (total_frames-3)*res_x_*res_y_);

        output_file.write((char*)write_head, (total_frames-3)*res_x_*res_y_);
    }

    output_file.close();

    return;



    //output_file.write((char*)write_head, num_images_*res_x_*res_y_);
    //output_file.close();

    // Move to the next cine
    int next_cine = (current_cine + 1)%5;
    if(next_cine == 0)
    {
        next_cine = 1;
    }
    std::string next_cine_str = std::to_string(next_cine);

    // Start recording in next cine
    //std::cout << tcplayer_.send_command("rec "+next_cine_str+"\r") << std::endl;




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

    //std::cout << tcplayer_.send_command("get info.*\r") << std::endl;

    //tcplayer_.send_command("get info.cinemem\r");

    return;
}


void CameraModel::set_live_parameters(int frame_rate, int exposure_time, int res_x, int res_y)
{
    frame_rate_ = frame_rate;
    exposure_time_ = exposure_time;
    res_x_ = res_x;
    res_y_ = res_y;
    image_size_ = res_x_*res_y_;
    std::cout << "a" << std::endl;
    live_image_buffer_.resize(res_x_*res_y_);      // Buffer needs to be resized.
    std::cout << "b" << std::endl;
    live_image_pointer_ = &live_image_buffer_[0];  // Buffer head needs to be put back to beginning.
    std::cout << "c" << std::endl;
    recorded_image_buffer_.resize(image_size_*num_images_);
    std::cout << "d" << std::endl;
    set_live_parameters_camera();


    return;
}

void CameraModel::set_default_parameters_camera(){
    std::string cmd = "set defc.* {";
    cmd += "res:" + std::to_string(res_x_) + "x" + std::to_string(res_y_)+", ";
    cmd += "rate:" + std::to_string(frame_rate_)+", ";
    cmd += "ptframes:0, ";
    cmd += "exp:" + std::to_string(exposure_time_)+"}\r";



    tcplayer_.send_command(cmd);

    return;
}

void CameraModel::set_live_parameters_camera()
{


    // Allocate memory on camera
    int cine_size = image_size_*num_images_/1000000.;   // cine size is size of image, times number of frames to record, in MB (divide by mil)
    std::string cine_size_str = std::to_string(cine_size);
    std::cout << "Allocating cines " <<
                 tcplayer_.send_command("alloc {"+ cine_size_str + "}\r") << std::endl;//", " + cine_size_str + ", " + cine_size_str + ", " + cine_size_str + "}\r") <<
                 //std::endl;



    std::string cmd = "set defc.* {";
    cmd += "res:" + std::to_string(res_x_) + "x" + std::to_string(res_y_)+", ";
    cmd += "rate:" + std::to_string(frame_rate_)+", ";
    cmd += "ptframes:0, ";
    cmd += "exp:" + std::to_string(exposure_time_)+"}\r";



    tcplayer_.send_command(cmd);



    tcplayer_.send_command("rec 1\r");

    return;
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

