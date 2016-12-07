#ifndef TCPLAYER_H
#define TCPLAYER_H

// Standard library
#include <string>

// Windows
#include <winsock2.h>

class TCPLayer
{

public:
    TCPLayer();
    ~TCPLayer();

    // TCP related
    std::string camera_ip_;
    std::string server_ip_;

    SOCKET command_socket_;
    SOCKET listen_socket_;
    SOCKET data_socket_;

    // Other
    bool initialized_;

private:
    // Initialization
    void initialize();
    bool start_winsock();
    bool open_command_socket();
    bool open_listen_socket();
    bool open_data_socket();

    // Commands


    // Closing
    void close();
    void stop_winsock();
    void close_command_socket();
    void close_listen_socket();
    void close_data_socket();

};

#endif // TCPLAYER_H
