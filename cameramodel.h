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
    void set_parameters(int frame_rate, int exposure_time, int res_x, int res_y);

signals:
    void state_update_live_image(int res_x, int res_y);


private:
    //////////
    // Objects
    //////////

    // TCP stuff
    TCPLayer tcplayer_;
    int command_port_;          // Port for sending/receiving commands, must be 7115
    int data_port_;             // Port on which to receive data stream; can be any open port.
    std::string camera_ip_;     // 100.100.37.9 (set by Phantom camera)
    std::string server_ip_;     // The IP address of the computer this softwarwe is running on
                                // Must be 100.100.100.1

    // Parameter stuff
    int frame_rate_;            // frames per second (fps)
    int exposure_time_;         // nanoseconds (ns)
    int image_size_;
    int res_x_;
    int res_y_;



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
