QT -= gui
QT+=sql
CONFIG += c++11 console
CONFIG -= app_bundle
DEFINES += QT_DEPRECATED_WARNINGS
SOURCES += \
        database.cpp \
        main.cpp

HEADERS += \
    database.h
