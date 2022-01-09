QT       += core gui sql

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++11

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    BLL/DatabaseHelper/databasehelper.cpp \
    DAL/Database/database.cpp \
    UI/DatabaseUi/databaseui.cpp \
    UI/MainPage/mainpage.cpp \
    UI/errorui.cpp \
    main.cpp

HEADERS += \
    BLL/DatabaseHelper/databasehelper.h \
    DAL/Database/database.h \
    Shared/student.h \
    UI/DatabaseUi/databaseui.h \
    UI/MainPage/mainpage.h \
    UI/errorui.h

FORMS += \
    UI/DatabaseUi/databaseui.ui \
    UI/MainPage/mainpage.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

DISTFILES += \
    README.md \
    assets/Icon-Transparent.ico \
    assets/Icon-WithBackground-Rounded.png \
    assets/Icon-WithBackground.ico \
    assets/Icon-WithBackground.png

RESOURCES += \
    resources.qrc
