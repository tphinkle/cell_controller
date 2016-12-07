// Program specific
#include "tcplayer.h"

// Standard library
#include <iostream>
#include <vector>

TCPLayer::TCPLayer()
{
    initialized_ = false;
    initialize();
}

TCPLayer::~TCPLayer()
{
    initialized_ = false;
    close();
}


///////////////////////////////////////////////
/// Initialization
///////////////////////////////////////////////

void TCPLayer::initialize()
{
    bool success;

    std::cout << "Initializing the TCP layer." << std::endl;
    if(!start_winsocket())
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

    std::cout << "\t\tWinsock opened." << std::endl;

    return true;
}

bool TCPLayer::open_command_socket()
{
    std::cout << "\tTrying to open command socket." << std::endl;

    std::cout << "\t\tCommand socket opened." << std::endl;

    return true;
}

bool TCPLayer::open_listen_socket()
{
    std::cout << "\tTrying to open listen socket." << std::endl;

    std::cout << "\t\tListen socket opened." << std::endl;

    return true;
}

bool TCPLayer::open_data_socket()
{
    std::cout << "\tTrying to open data socket." << std::endl;

    std::cout << "\t\tData socket opened." << std::endl;

    return true;
}

///////////////////////////////////////////////
/// Commands
///////////////////////////////////////////////

void TCPLayer::send_command(std::string command)
{
    return;
}

void TCPLayer::send_data_request(std::string request, std::vector<char>& data_buffer)
{

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
