#-------------------------------------------------
#
# Project created by QtCreator 2017-01-11T19:09:51
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = qt_time_tracker
TEMPLATE = app


SOURCES += main.cpp\
	mainwindow.cpp \
    timer_button.cpp \
    window_stats.cpp \
    timer_data.cpp

HEADERS  += mainwindow.h \
    timer_button.h \
    window_stats.h \
    timer_data.h \
    time_helper.h \

FORMS    += mainwindow.ui \
    window_stats.ui
