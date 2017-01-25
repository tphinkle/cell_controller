// Program specific
#include "tcplayer.h"

// Standard library
#include <iostream>
#include <vector>

TCPLayer::TCPLayer()
{
    initialized_ = false;
}

TCPLayer::~TCPLayer()
{
    initialized_ = false;
    close();
}


///////////////////////////////////////////////
/// Initialization
///////////////////////////////////////////////

void TCPLayer::initialize(int command_port, int data_port, std::string camera_ip, std::string server_ip)
{
    command_port_ = command_port;
    data_port_ = data_port;
    camera_ip_ = camera_ip;
    server_ip_ = server_ip;

    bool success;

    std::cout << "Initializing the TCP layer." << std::endl;
    if(!start_winsock())
    {
        std::cout << "Failed to initialize TCP layer." << std::endl;
        initialized_ = false;
        return;
    }

    if(!open_command_socket())
    {
        std::cout << "Failed to initialize TCP layer." << std::endl;
        initialized_ = false;
        return;
    }

    if(!open_listen_socket())
    {
        std::cout << "Failed to initialize TCP layer." << std::endl;
        initialized_ = false;
        return;
    }

    if(!open_data_socket())
    {
        std::cout << "Failed to initialize TCP layer." << std::endl;
        initialized_ = false;
        return;
    }

    std::cout << "\tTCP layer initialized." << std::endl;
    initialized_ = true;
    return;

}

bool TCPLayer::start_winsock()
{

    std::cout << "\tTrying to open winsock." << std::endl;
    WSAData wsadata;
    int result = WSAStartup(MAKEWORD(2,2), &wsadata);
    if(result != 0)
    {
        std::cout << "\t\tFailed to open Winsocket." << std::endl;
        return false;
    }

    std::cout  << "\t\tWinsocket opened." << std::endl;


    return true;
}

bool TCPLayer::open_command_socket()
{
    std::cout << "\tTrying to open command socket." << std::endl;

    command_socket_ = INVALID_SOCKET;
    command_socket_ = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);

    if(command_socket_ == INVALID_SOCKET)
    {
        std::cout << "\t\tFailed to open command socket: " << WSAGetLastError() << std::endl;
        return false;
    }

    SOCKADDR_IN target;
    target.sin_family = AF_INET;
    target.sin_port = htons(command_port_);
    target.sin_addr.s_addr = inet_addr(camera_ip_.c_str());

    int error = ::connect(command_socket_, (SOCKADDR*) &target, sizeof(target));
    if(error == SOCKET_ERROR)
    {
        std::cout << "\t\tFailed to connect command socket: " << WSAGetLastError() << std::endl;
        return false;
    }

    std::cout << "\t\tCommand socket opened." << std::endl;

    return true;
}

bool TCPLayer::open_listen_socket()
{
    // Tutorial on creating a socket for the server
    // https://msdn.microsoft.com/en-us/library/windows/desktop/bb530742(v=vs.85.aspx)

    std::cout << "\tTrying to open listen socket." << std::endl;

    struct addrinfo* result = NULL, *pytr = NULL, hints;

    ZeroMemory(&hints, sizeof(hints));
    hints.ai_family = AF_INET;
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_protocol = IPPROTO_TCP;
    hints.ai_flags = AI_PASSIVE;

    int error = getaddrinfo(NULL, std::to_string(data_port_).c_str(), &hints, &result);

    if(error != 0)
    {
        std::cout << "\t\tFailed to get address info: " << error << std::endl;
        return false;
    }

    listen_socket_ = INVALID_SOCKET;

    listen_socket_ = socket(result->ai_family, result->ai_socktype, result->ai_protocol);

    if(listen_socket_ == INVALID_SOCKET)
    {
        std::cout << "\t\tError opening socket: " << WSAGetLastError() << std::endl;
        freeaddrinfo(result);
        return false;
    }

    error = bind(listen_socket_, result->ai_addr, (int)result->ai_addrlen);
    if(error == SOCKET_ERROR)
    {
        std::cout << "\t\tFailed to bind socket: " << WSAGetLastError() << std::endl;
        freeaddrinfo(result);
        return false;
    }

    freeaddrinfo(result);

    if(listen(listen_socket_, SOMAXCONN) == SOCKET_ERROR)
    {
        std::cout << "\t\tFailed to listen: " << WSAGetLastError() << std::endl;
        return false;
    }

    std::cout << "\t\tSuccessfully opened listen socket." << std::endl;

    return true;
}

bool TCPLayer::open_data_socket()
{
    std::cout << "\tTrying to open data socket." << std::endl;

    std::cout << "\t\tData socket opened." << std::endl;

    return true;
}

void TCPLayer::set_data_socket_blocking(bool blocking)
{
    u_long mode;
    if(blocking)
    {
        mode = 0;
    }
    else
    {
        mode = 1;
    }

    ioctlsocket(data_socket_, FIONBIO, &mode);

    return;
}

///////////////////////////////////////////////
/// Commands
///////////////////////////////////////////////

std::string TCPLayer::send_command(std::string command)
{
    //std::cout << "Trying to send command: " << std::endl << "\t" << command << std::endl;

    if(command_socket_ == INVALID_SOCKET){
        std::cout << "Command socket not open!" << std::endl;
        return 0x00;
    }

    send(command_socket_, command.c_str(), command.length(), NULL);

    char recv_buffer[10000];
    int bytes = recv(command_socket_, recv_buffer, sizeof(recv_buffer), NULL);

    return std::string(recv_buffer).substr(0, bytes);

}

void TCPLayer::send_data_request(std::string request, std::vector<uchar>& data_buffer)
{


    send_command(request);



    static bool listen_set = false;
    if(listen_set == false)
    {
        data_socket_ = INVALID_SOCKET;

        data_socket_ = accept(listen_socket_, NULL, NULL);

        listen_set = true;
    }


    int data_buffer_iter = 0;

    int bytes_returned;

    set_data_socket_blocking(false);


    while(data_buffer_iter < data_buffer.size())
    {
        bytes_returned = recv(data_socket_, (char*)&(data_buffer[data_buffer_iter]), data_buffer.size()-data_buffer_iter, 0);


        if(bytes_returned > 0){
            data_buffer_iter += bytes_returned;
        }




    }



    set_data_socket_blocking(true);

    return;
}

void TCPLayer::send_data_request(std::string request, std::vector<uchar>& data_buffer, int size)
{
    std::cout << "size = " <<   size << std::endl;
    //return;
    std::cout << send_command(request) << std::endl;



    int data_buffer_iter = 0;
    int bytes_returned;

    set_data_socket_blocking(false);


    while(data_buffer_iter < size)
    {
        //std::cout << data_buffer_iter << std::endl;
        bytes_returned = recv(data_socket_, (char*)&(data_buffer[data_buffer_iter]), size-data_buffer_iter, 0);

        if(bytes_returned > 0){
            data_buffer_iter += bytes_returned;
        }



    }


    std::cout << "1" << std::endl;

    set_data_socket_blocking(true);

    return;
}


///////////////////////////////////////////////
/// Closing
///////////////////////////////////////////////

void TCPLayer::close()
{
   std::cout << "Cleaning up TCP layer." << std::endl;

   close_command_socket();
   close_listen_socket();
   close_data_socket();
   stop_winsock();

   return;
}

void TCPLayer::stop_winsock()
{
    std::cout << "\tStopping winsock." << std::endl;
    WSACleanup();
    return;
}

void TCPLayer::close_command_socket()
{
    std::cout << "\tClosing command socket." << std::endl;
    if(command_socket_)
    {
        closesocket(command_socket_);
    }
    return;
}

void TCPLayer::close_listen_socket()
{
    std::cout << "\tClosing listen socket." << std::endl;
    if(listen_socket_)
    {
        closesocket(listen_socket_);
    }
    return;
}

void TCPLayer::close_data_socket()
{
    std::cout << "\tClosing data socket." << std::endl;
    if(data_socket_)
    {
        closesocket(data_socket_);
    }
    return;
}
