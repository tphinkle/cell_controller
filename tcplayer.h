#ifndef TCPLAYER_H
#define TCPLAYER_H

// Standard library
#include <string>

// Windows
#include <winsock2.h>
#include <Ws2tcpip.h>

class TCPLayer
{

public:
    TCPLayer();
    ~TCPLayer();

    // TCP related
    int command_port_;
    int data_port_;

    std::string camera_ip_;
    std::string server_ip_;


    SOCKET command_socket_;
    SOCKET listen_socket_;
    SOCKET data_socket_;

    // Other
    bool initialized_;

private:
    // Initialization
    void initialize(int command_port, int data_port, std::string camera_ip, std::string server_ip);
    bool start_winsock();
    bool open_command_socket();
    bool open_listen_socket();
    bool open_data_socket();
    void set_data_socket_blocking(bool blocking);

    // Commands
    std::string send_command(std::string command);
    void send_data_request(std::string command, std::vector<char> buffer);

    // Closing
    void close();
    void stop_winsock();
    void close_command_socket();
    void close_listen_socket();
    void close_data_socket();

};

#endif // TCPLAYER_H
