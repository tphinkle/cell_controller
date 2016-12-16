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

    uchar* live_image_pointer_;
    std::vector<uchar> live_image_buffer_;

public slots:
    void get_live_image();

signals:
    void state_update_live_image();


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



    /////////////
    // Functions
    /////////////

    void start_data_stream();
    int get_live_cine_number();


    // Misc. functions
    std::vector<std::string> split_string(std::string str, std::string split_str);
};

#endif // CAMERAMODEL_H
