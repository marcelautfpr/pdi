#-------------------------------------------------
#
# Project created by QtCreator 2016-07-03T21:44:06
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = ApsFinal
TEMPLATE = app

CONFIG += c++14

INCLUDEPATH += C:\opencv\build\install\include
LIBS += -LC:\opencv\build\install\x86\mingw\lib \
    -llibopencv_calib3d310 \
    -llibopencv_core310 \
    -llibopencv_features2d310 \
    -llibopencv_flann310 \
    -llibopencv_highgui310 \
    -llibopencv_imgcodecs310 \
    -llibopencv_imgproc310 \
    -llibopencv_ml310 \
    -llibopencv_objdetect310 \
    -llibopencv_photo310 \
    -llibopencv_shape310 \
    -llibopencv_stitching310 \
    -llibopencv_superres310 \
    -llibopencv_video310 \
    -llibopencv_videoio310 \
    -llibopencv_videostab310

SOURCES += main.cpp\
        mainwindow.cpp \
    perceptron.cpp \
    matrizcoocorrencia.cpp \
    lpb.cpp \
    knn.cpp \
    kmeans.cpp \
    fractais.cpp \
    estatisitica.cpp \
    descritor.cpp \
    dadosimagens.cpp \
    classificadores.cpp \
    matrizconfusaosvm.cpp

HEADERS  += mainwindow.h \
    perceptron.h \
    matrizcoocorrencia.h \
    lpb.h \
    knn.h \
    kmeans.h \
    fractais.h \
    estatisitica.h \
    descritor.h \
    dadosimagens.h \
    classificadores.h \
    matrizconfusaosvm.h

FORMS    += mainwindow.ui

DISTFILES += \
    QmlTeste.qml
