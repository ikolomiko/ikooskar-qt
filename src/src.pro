QT       += core gui sql

#TRANSLATIONS    +=  gui_tr.ts

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17

TARGET = ikoOSKAR

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    BLL/DatabaseHelper/databasehelper.cpp \
    BLL/StudentEditor/studenteditor.cpp \
    DAL/Database/database.cpp \
    UI/Common/module.cpp \
    UI/DatabaseUi/databaseui.cpp \
    UI/MainPage/mainpage.cpp \
    UI/ErrorUi/errorui.cpp \
    UI/StudentEditorUi/studenteditorui.cpp \
    UI/WelcomeUi/welcomeui.cpp \
    main.cpp

HEADERS += \
    BLL/DatabaseHelper/databasehelper.h \
    BLL/StudentEditor/studenteditor.h \
    DAL/Database/database.h \
    Shared/student.h \
    UI/Common/module.h \
    UI/DatabaseUi/databaseui.h \
    UI/MainPage/mainpage.h \
    UI/ErrorUi/errorui.h \
    UI/StudentEditorUi/studenteditorui.h \
    UI/WelcomeUi/welcomeui.h

FORMS += \
    UI/DatabaseUi/databaseui.ui \
    UI/MainPage/mainpage.ui \
    UI/StudentEditorUi/studenteditorui.ui \
    UI/WelcomeUi/welcomeui.ui

RESOURCES += \
    ../assets/resources.qrc

QMAKE_CXXFLAGS += -Wall -Wextra -Werror
