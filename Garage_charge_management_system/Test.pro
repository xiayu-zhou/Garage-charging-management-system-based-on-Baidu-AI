#-------------------------------------------------
#
# Project created by QtCreator 2024-05-29T10:31:07
#
#-------------------------------------------------

QT       += core gui network texttospeech sql

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = Test
TEMPLATE = app

# The following define makes your compiler emit warnings if you use
# any feature of Qt which has been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

CONFIG += c++11

SOURCES += \
        main.cpp \
        widget.cpp \
    mysql_x.cpp \
    MyThread.cpp

HEADERS += \
        widget.h \
    mysql_x.h \
    MyThread.h

FORMS += \
        widget.ui



INCLUDEPATH += /home/zxy/opencv/install/include \
                /home/zxy/opencv/install/include/opencv4 \
                /home/zxy/opencv/install/include/opencv4/opencv2\
                /home/zxy/tools/jsoncpp/include                 \
                /home/zxy/tools/jsoncpp/include/json            \
                /home/zxy/tools/curl-7.38.0/build/include       \
                /home/zxy/tools/openssl-1.0.2k/build/include    \
                /home/zxy/tools/openssl-1.0.2k/build/include/openssl
               # /home/zxy/tools/ekho-9.0/build/include


LIBS += /home/zxy/opencv/install/lib/libopencv_* \
        /home/zxy/tools/jsoncpp/build/debug/lib/*.a \
        /home/zxy/tools/curl-7.38.0/build/lib/*.a  \
        /home/zxy/tools/openssl-1.0.2k/build/lib/*.a
     #   /home/zxy/tools/ekho-9.0/build/lib/*.a



INCLUDEPATH +=/home/zxy/tools/onnxruntime/install/include


LIBS += /home/zxy/tools/onnxruntime/install/lib/libonnxruntime*


# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target
