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
    ~CameraModel();

    uchar* live_image_pointer_;
    std::vector<uchar> live_image_buffer_;
    std::vector<uchar> recorded_image_buffer_;

    void set_live_parameters_camera();



public slots:
    void get_live_image();
    void set_live_parameters(int frame_rate, int exposure_time, int res_x, int res_y);

    void get_cine_info();


    // Get parameters from camera
    int get_live_cine_number();
    int get_cine_frcount(int cine_number);
    int get_cine_firstfr(int cine_number);
    int get_cine_lastfr(int cine_number);

    // Set parameters on camera
    void set_resolution(int res_x, int res_y);
    void set_default_parameters_camera();

    // Camera requests
    void start_data_stream();
    void record();


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
                                // Must be 100.100.100.1.
                                // Computer must have gigabit ethernet interface!

    // Parameter stuff
    // Camera
    int frame_rate_;            // frames per second (fps)
    int exposure_time_;         // nanoseconds (ns)
    int image_size_;            // Image area, product of res_x, res_y
    int num_images_;            // Number of requested images per record
    int res_x_;
    int res_y_;
    int max_memory_;            // Max memory capable of being stored by camera in MB

    // Computer
    int max_buffer_size_;       // Max memory on computer to reserve for buffer in MB



    // Data stuff



    /////////////
    // Functions
    /////////////






    // Misc. functions
    std::vector<std::string> split_string(std::string str, std::string split_str);
};

#endif // CAMERAMODEL_H
