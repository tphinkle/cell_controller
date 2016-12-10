#ifndef SERIALCONNECTION_H
#define SERIALCONNECTION_H

#include <windows.h>             // For serial connection.
#include <string>

class SerialConnection
{
public:
    SerialConnection();
    ~SerialConnection();

    std::string write(std::string cmd);
    std::string write_data_request(std::string cmd);
    std::string read();
    std::string read_data();

private:
    HANDLE serial_handle_;
};

#endif // SERIALCONNECTION_H
