#-------------------------------------------------
#
# Project created by QtCreator 2018-12-06T11:32:40
#
#-------------------------------------------------

QT       += core gui sql

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = ObjectDetection
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

CONFIG += c++11    # lambda 

# cuda
#INCLUDEPATH += /usr/local/cuda-8.0/include

#INCLUDEPATH += /usr/lib/x86_64-linux-gnu
#LIBS += -L/usr/lib/x86_64-linux-gnu 
#LIBS += -ltiff
 
# other dependencies
#LIBS += -lglog -lgflags -lprotobuf -lboost_system -lboost_thread \
#      -llmdb -lleveldb -lstdc++ -lcudnn -lcblas -latlas \
#      -lswscale -lgomp -lpthread -luuid


# opencv
INCLUDEPATH += /usr/local/include \
            /usr/local/include/opencv \
            /usr/local/include/opencv2
LIBS += -L/usr/local/lib
LIBS += -lopencv_core -lopencv_imgproc -lopencv_highgui -lopencv_imgcodecs -lopencv_features2d

# verifier : Json common and Net czmq
INCLUDEPATH += deps/include \
            deps/include/common \
            deps/include/czmq
LIBS += -L deps/lib
LIBS += -lzmq -lczmq -lcommon


SOURCES += \
        main.cpp \
        mainwindow.cpp \
    clickablelabel.cpp \
    loginwindow.cpp \
    cfg/config.cpp \
    Detect/base64.cpp \
    Detect/parseJson.cpp \
    Detect/detectthread.cpp \
    Detect/sqldatabasehelper.cpp \ 
    objlabel.cpp \
    analysis.cpp

HEADERS += \
        mainwindow.h \
    clickablelabel.h \
    loginwindow.h \
    cfg/config.h \
    Detect/base64.h \
    Detect/parseJson.h \
    Detect/detectthread.h \
    Detect/sqldatabasehelper.h \ 
    objlabel.h \
    analysis.h
    


# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

RESOURCES += \
    main.qrc

DISTFILES +=
