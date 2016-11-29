#include "serialconnection.h"

#include <iostream>

#include <string>


SerialConnection::SerialConnection()
{
    // Serial instructions in windows found at stackoverflow.com/questions/15794422/serial-port-rs-232-connection-in-c

    // Open serial port
    serial_handle_ = CreateFile(L"\\\\.\\COM4", GENERIC_READ | GENERIC_WRITE, 0, NULL, OPEN_EXISTING, 0, NULL);


    // Basic settings
    // Device Control Block (DCB) documentation found at
    // https://msdn.microsoft.com/en-us/library/windows/desktop/aa363214(v=vs.85).aspx
    DCB serial_params = { 0 };

    serial_params.DCBlength = sizeof(serial_params);

    GetCommState(serial_handle_, &serial_params);

    serial_params.BaudRate = CBR_9600;
    serial_params.ByteSize = 8;
    serial_params.StopBits = TWOSTOPBITS;
    serial_params.Parity = NOPARITY;
    SetCommState(serial_handle_, &serial_params);

    // Set timeouts
    COMMTIMEOUTS timeout = { 0 };
    timeout.ReadIntervalTimeout = MAXDWORD;
    timeout.ReadTotalTimeoutConstant = 5;
    timeout.ReadTotalTimeoutMultiplier = 5;
    timeout.WriteTotalTimeoutConstant = 0;
    timeout.WriteTotalTimeoutMultiplier = 0;

    SetCommTimeouts(serial_handle_, &timeout);

    // Setup event system for receiving IO status
    // Code follows approach at
    // https://www.codeproject.com/articles/2682/serial-communication-in-windows
    // or alternatively
    // https://msdn.microsoft.com/en-us/library/ff802693.aspx

    SetCommMask(serial_handle_, EV_TXEMPTY | EV_RXCHAR);

}

SerialConnection::~SerialConnection()
{
    CloseHandle(serial_handle_);
}


std::string SerialConnection::write(std::string cmd)
{
    DWORD bytes_written;

    char cmd_buf[cmd.size()];
    strcpy(cmd_buf, cmd.c_str());

    bool return_value = WriteFile(serial_handle_, &cmd_buf, cmd.size(), &bytes_written, NULL);

    return read();
}

std::string SerialConnection::read()
{
    int len = 100;
    char buf[100];

    DWORD bytes_read;


    PurgeComm(serial_handle_, PURGE_RXCLEAR); // Purge input buffer before reading

    do
    {
        //std::cout << "Trying to read a byte." << std::endl;
        ReadFile(serial_handle_, &buf, sizeof(buf), &bytes_read, NULL);
    } while(bytes_read > 0);

    std::string read_value = std::string(buf);
    std::string return_value;

    for(size_t i = 0; i < read_value.size(); i++)
    {
        if (read_value[i] == '<')
        {
            return_value += '<';
        }

        else if (read_value[i] == '>')
        {
            return_value += '>';
        }

        else if (read_value[i] == ':')
        {
            return_value += ':';
        }
    }


    //std::cout << "this is the final byte" << return_value << "\n";

    return return_value;
}
