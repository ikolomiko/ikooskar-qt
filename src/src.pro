QT       += core gui sql

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17

TARGET = ikoOSKAR

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    BLL/DatabaseHelper/databasehelper.cpp \
    DAL/Database/database.cpp \
    UI/DatabaseUi/databaseui.cpp \
    UI/MainPage/mainpage.cpp \
    UI/ErrorUi/errorui.cpp \
    UI/WelcomeUi/welcomeui.cpp \
    main.cpp

HEADERS += \
    BLL/DatabaseHelper/databasehelper.h \
    DAL/Database/database.h \
    Shared/student.h \
    UI/DatabaseUi/databaseui.h \
    UI/MainPage/mainpage.h \
    UI/ErrorUi/errorui.h \
    UI/WelcomeUi/welcomeui.h

FORMS += \
    UI/DatabaseUi/databaseui.ui \
    UI/MainPage/mainpage.ui \
    UI/WelcomeUi/welcomeui.ui

RESOURCES += \
    ../assets/resources.qrc

QMAKE_CXXFLAGS += -Wall -Wextra -Werror
