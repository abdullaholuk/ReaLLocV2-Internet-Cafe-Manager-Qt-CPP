#-------------------------------------------------
#
#author : Abdullah Oluk
#title  : ReaLLocV2
#school : Yildiz Technical University / Computer Engineering ( Istanbul / Turkey )
#e-mail : abdullaholuk123@gmail.com
#This is a internet cafe management system interface without LAN connection of computers
#
#-------------------------------------------------

QT       += core gui sql

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = ReaLLocV2
TEMPLATE = app

# The following define makes your compiler emit warnings if you use
# any feature of Qt which as been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0


SOURCES += main.cpp\
        mainwindow.cpp \
    computer.cpp \
    startcomputer.cpp \
    user.cpp \
    subscriber.cpp \
    managedb.cpp \
    showsubscribertable.cpp \
    showdailyincometable.cpp

HEADERS  += mainwindow.h \
    computer.h \
    startcomputer.h \
    user.h \
    subscriber.h \
    managedb.h \
    showsubscribertable.h \
    showdailyincometable.h

FORMS    += mainwindow.ui \
    startcomputer.ui \
    showsubscribertable.ui \
    showdailyincometable.ui

RESOURCES += \
    resources.qrc
