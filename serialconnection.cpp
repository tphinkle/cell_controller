// Program specific
#include "serialconnection.h"


// Standard library
#include <iostream>
#include <string>
#include <vector>
#include <algorithm>      // Needed for std::find




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

    PurgeComm(serial_handle_, PURGE_RXCLEAR);  // Maybe have to remove this

    bool return_value = WriteFile(serial_handle_, &cmd_buf, cmd.size(), &bytes_written, NULL);

    return read();
}

std::string SerialConnection::write_data_request(std::string cmd)
{
    DWORD bytes_written;

    char cmd_buf[cmd.size()];
    strcpy(cmd_buf, cmd.c_str());

    PurgeComm(serial_handle_, PURGE_RXCLEAR);  // Maybe have to remove this

    bool return_value = WriteFile(serial_handle_, &cmd_buf, cmd.size(), &bytes_written, NULL);
    return read_data();
}

std::string SerialConnection::read()
{
    int len = 100;
    char buf[100];

    DWORD bytes_read;


    PurgeComm(serial_handle_, PURGE_RXCLEAR); // Purge input buffer before reading

    do
    {
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





    return return_value;
}

std::string SerialConnection::read_data()
{

    std::vector<unsigned char> buf(100);

    DWORD bytes_read = 0;
    DWORD total_bytes_read = 0;


    PurgeComm(serial_handle_, PURGE_RXABORT); // Purge input buffer before reading
    PurgeComm(serial_handle_, PURGE_RXCLEAR); // Purge input buffer before reading


    do // Read all bytes
    {
        ReadFile(serial_handle_, &buf[bytes_read], sizeof(buf), &bytes_read, NULL);
        total_bytes_read = total_bytes_read + bytes_read;
        //std::cout << "read result = " << std::string(buf.begin(), buf.end()) << std::endl;
        //std::cout << "bytes read " << bytes_read << std::endl;
    } while(bytes_read > 0);


    // Find where the colon/</> is, which indicates where the syringe rate can be found within the string
    unsigned int colon_index;
    for(unsigned int i = 0; i < buf.size(); i++)
    {
        if((buf[i] == ":"[0]) || (buf[i] == "<"[0]) || (buf[i] == ">"[0]))
        {
            colon_index = i;
            break;
        }

    }


    std::string read_value(buf.begin()+colon_index - 5, buf.begin() + colon_index);
    return read_value;

}
