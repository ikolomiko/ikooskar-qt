QT       += core gui sql

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
    UI/AboutUi/aboutui.cpp \
    UI/Common/module.cpp \
    UI/DatabaseUi/databaseui.cpp \
    UI/HistoryUi/historyui.cpp \
    UI/MainPage/mainpage.cpp \
    UI/ErrorUi/errorui.cpp \
    UI/MultiImportUi/multiimportui.cpp \
    UI/SchemeGeneratorUi/schemegeneratorui.cpp \
    UI/StudentEditorUi/studenteditorui.cpp \
    UI/WelcomeUi/welcomeui.cpp \
    main.cpp

HEADERS += \
    BLL/DatabaseHelper/databasehelper.h \
    BLL/StudentEditor/studenteditor.h \
    DAL/Database/database.h \
    Shared/student.h \
    UI/AboutUi/aboutui.h \
    UI/Common/module.h \
    UI/DatabaseUi/databaseui.h \
    UI/HistoryUi/historyui.h \
    UI/MainPage/mainpage.h \
    UI/ErrorUi/errorui.h \
    UI/MultiImportUi/multiimportui.h \
    UI/SchemeGeneratorUi/schemegeneratorui.h \
    UI/StudentEditorUi/studenteditorui.h \
    UI/WelcomeUi/welcomeui.h

FORMS += \
    UI/AboutUi/aboutui.ui \
    UI/DatabaseUi/databaseui.ui \
    UI/HistoryUi/historyui.ui \
    UI/MainPage/mainpage.ui \
    UI/MultiImportUi/multiimportui.ui \
    UI/SchemeGeneratorUi/schemegeneratorui.ui \
    UI/StudentEditorUi/studenteditorui.ui \
    UI/WelcomeUi/welcomeui.ui

RESOURCES += \
    ../assets/resources.qrc

win32:RC_ICONS = ../assets/multires.ico

QMAKE_CXXFLAGS += -Wall -Wextra -Werror
