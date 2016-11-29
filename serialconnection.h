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
    std::string read();

private:
    HANDLE serial_handle_;
};

#endif // SERIALCONNECTION_H
