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


private:
    TCPLayer tcplayer_;

    std::vector<char> live_image_;
    char* live_image_pointer_;
};

#endif // CAMERAMODEL_H
