#-------------------------------------------------
#
# MaQuis
#
# Copyright 2016 Alexander Koch <lynix47@gmail.com>
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = bin/MaQuis
TEMPLATE = app

SOURCES += src/main.cpp src/mainwindow.cpp
HEADERS  += src/mainwindow.h

FORMS    += ui/mainwindow.ui

unix: CONFIG += link_pkgconfig
unix: PKGCONFIG += libturbojpeg

