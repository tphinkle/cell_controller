#include "cameramodel.h"



// Standard library
#include <iostream>
#include <string>

CameraModel::CameraModel()
{
    command_port_ = 7115;
    data_port_ = 15234;

    camera_ip_ = "100.100.37.9";
    server_ip_ = "100.100.100.1";

    res_x_ = 640;
    res_y_ = 480;
    frame_rate_ = 10000;
    exposure_time_ = 10000;

    image_size_ = res_x_*res_y_;
    live_image_buffer_.resize(image_size_);
    live_image_pointer_ = &live_image_buffer_[0];

    tcplayer_.initialize(command_port_, data_port_, camera_ip_, server_ip_);



    start_data_stream();

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
    std::cout << "Trying to get a live image..." << std::endl;
    std::string req = "img {cine:-1, start:-1, cnt:1}\r";
    tcplayer_.send_data_request(req, live_image_buffer_);

    std::cout << "Trying to emit the signal" << std::endl;

    emit(state_update_live_image(res_x_, res_y_));

    std::cout << "Emit the signal" << std::endl;

    return;
}

int CameraModel::get_live_cine_number()
{

    std::cout << "Trying to get live cine number..." << std::endl;
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

void CameraModel::set_parameters(int frame_rate, int exposure_time, int res_x, int res_y)
{
    frame_rate_ = frame_rate;
    exposure_time_ = exposure_time;
    res_x_ = res_x;
    res_y_ = res_y;
    live_image_buffer_.resize(res_x_*res_y_);
    live_image_pointer_ = &live_image_buffer_[0];

    std::string cmd = "set defc.* {";
    cmd += "res:" + std::to_string(res_x) + "x" + std::to_string(res_y)+", ";
    cmd += "rate:" + std::to_string(frame_rate)+", ";
    cmd += "exp:" + std::to_string(exposure_time)+"}\r";

    std::cout << "Parameter command: " << cmd << std::endl;


    tcplayer_.send_command(cmd);


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

