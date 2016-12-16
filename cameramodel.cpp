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

    image_w_ = 640;
    image_h_ = 480;
    image_size_ = image_w_*image_h_;
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

    emit(state_update_live_image());
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

