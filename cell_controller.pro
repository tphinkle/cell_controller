#-------------------------------------------------
#
# Project created by QtCreator 2016-11-28T11:02:24
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = cell_controller
TEMPLATE = app

# Needed according to
# forums.ni.com/t5/Multifunction-DAQ/Howto-use-NIDAQmx-with-mingw-gcc-3-4-2/m-p/294361?jump=true
LIBS += "C:\Program Files\National Instruments\NI-DAQ\DAQmx ANSI C Dev\lib\msvc\NIDAQmx.lib" \
        -lws2_32


SOURCES += main.cpp\
    syringecontroller.cpp \
    maincontroller.cpp \
    mainmodel.cpp \
    mainview.cpp \
    syringemodel.cpp \
    rpmodel.cpp \
    rpcontroller.cpp \
    serialconnection.cpp \
    rpthreadcontroller.cpp \
    cameracontroller.cpp \
    cameramodel.cpp \
    tcplayer.cpp

HEADERS  += \
    syringecontroller.h \
    maincontroller.h \
    mainmodel.h \
    mainview.h \
    syringemodel.h \
    rpmodel.h \
    rpcontroller.h \
    serialconnection.h \
    "C:\Program Files\National Instruments\NI-DAQ\DAQmx ANSI C Dev\include\NIDAQmx.h" \
    rpthreadcontroller.h \
    cameracontroller.h \
    cameramodel.h \
    tcplayer.h

FORMS    += \
    mainview.ui

#INCLUDEPATH += C:\Program Files\National$$Instruments\NI-DAQ\DAQmx$$ANSI$$C$$Dev

CONFIG += qwt

DEFINES += QWT_DLL
