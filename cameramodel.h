#ifndef CAMERAMODEL_H
#define CAMERAMODEL_H

// Program specific
#include <tcplayer.h>

// Qt
#include <QObject>



class CameraModel : public QObject
{
    Q_OBJECT



public:
    CameraModel();


private:
    //////////
    // Objects
    //////////

    // TCP stuff
    TCPLayer tcplayer_;
    int command_port_;
    int data_port_;
    std::string camera_ip_;
    std::string server_ip_;

    // Parameter stuff
    int image_w_;
    int image_h_;
    int image_size_;



    // Data stuff
    std::vector<char> live_image_buffer_;
    char* live_image_pointer_;

    /////////////
    // Functions
    /////////////

    void start_data_stream();
    void get_live_frame();
    int get_live_cine_number();

    // Misc. functions
    std::vector<std::string> split_string(std::string str, std::string split_str);
};

#endif // CAMERAMODEL_H
