# Generated from src.pro.

cmake_minimum_required(VERSION 3.16)
project(ikoOSKAR VERSION 1.0 LANGUAGES CXX)

set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_CXX_STANDARD 17)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

find_package(QT NAMES Qt6 REQUIRED COMPONENTS Widgets)
find_package(Qt6 REQUIRED COMPONENTS Widgets Sql)

set(PROJECT_SOURCES
    src/BLL/DatabaseHelper/databasehelper.cpp src/BLL/DatabaseHelper/databasehelper.h
    src/BLL/StudentEditor/studenteditor.cpp src/BLL/StudentEditor/studenteditor.h
    src/BLL/MultiImportHelper/multiimporthelper.h src/BLL/MultiImportHelper/multiimporthelper.cpp
    src/BLL/SchemeGenerator/schemegenerator.h src/BLL/SchemeGenerator/schemegenerator.cpp
    src/DAL/Database/database.cpp src/DAL/Database/database.h
    src/DAL/MultiImport/multiimport.h src/DAL/MultiImport/multiimport.cpp
    src/Shared/examstudent.h
    src/Shared/student.h
    src/UI/AboutUi/aboutui.cpp src/UI/AboutUi/aboutui.h src/UI/AboutUi/aboutui.ui
    src/UI/Common/module.cpp src/UI/Common/module.h
    src/UI/Common/spinner.h src/UI/Common/spinner.cpp src/UI/Common/spinner.ui
    src/UI/Common/twobuttonnav.h src/UI/Common/twobuttonnav.cpp
    src/UI/DatabaseUi/databaseui.cpp src/UI/DatabaseUi/databaseui.h src/UI/DatabaseUi/databaseui.ui
    src/UI/ErrorUi/errorui.cpp src/UI/ErrorUi/errorui.h
    src/UI/MainPage/mainpage.cpp src/UI/MainPage/mainpage.h src/UI/MainPage/mainpage.ui
    src/UI/MultiImportUi/multiimportui.cpp src/UI/MultiImportUi/multiimportui.h src/UI/MultiImportUi/multiimportui.ui
    src/UI/MultiImportUi/miclasspickerui.h src/UI/MultiImportUi/miclasspickerui.cpp src/UI/MultiImportUi/miclasspickerui.ui
    src/UI/MultiImportUi/mifilepickerui.h src/UI/MultiImportUi/mifilepickerui.cpp src/UI/MultiImportUi/mifilepickerui.ui
    src/UI/MultiImportUi/mipreviewui.h src/UI/MultiImportUi/mipreviewui.cpp src/UI/MultiImportUi/mipreviewui.ui
    src/UI/SchemesUi/newschemedialog.h src/UI/SchemesUi/newschemedialog.cpp src/UI/SchemesUi/newschemedialog.ui
    src/UI/SchemesUi/nsclasspickerui.h src/UI/SchemesUi/nsclasspickerui.cpp src/UI/SchemesUi/nsclasspickerui.ui
    src/UI/SchemesUi/nsexaminfoui.h src/UI/SchemesUi/nsexaminfoui.cpp src/UI/SchemesUi/nsexaminfoui.ui
    src/UI/SchemesUi/schemesui.cpp src/UI/SchemesUi/schemesui.h src/UI/SchemesUi/schemesui.ui
    src/UI/StudentEditorUi/studenteditorui.cpp src/UI/StudentEditorUi/studenteditorui.h src/UI/StudentEditorUi/studenteditorui.ui
    src/UI/WelcomeUi/welcomeui.cpp src/UI/WelcomeUi/welcomeui.h src/UI/WelcomeUi/welcomeui.ui
    src/main.cpp
    assets/icon.rc
)

include_directories("src")


qt_add_executable(ikoOSKAR
    MANUAL_FINALIZATION
    ${PROJECT_SOURCES}
)


target_link_libraries(ikoOSKAR PRIVATE
    Qt6::Widgets
    Qt6::Core
    Qt6::Gui
    Qt6::Sql
)

target_compile_options(ikoOSKAR PRIVATE
    -Wall
    -Werror
    -Wextra
)


# Resources:
set(resources_resource_files
    "assets/Icon-Transparent.png"
    "assets/Icon-WhiteBGx128.png"
    "assets/Icon-WhiteBGx64.png"
    "assets/arrow-right.png"
    "assets/arrow-left.png"
    "assets/add.png"
    "assets/banner.png"
    "assets/check.png"
    "assets/edit.png"
    "assets/help.png"
    "assets/history.png"
    "assets/home.png"
    "assets/info.png"
    "assets/more.png"
    "assets/new-scheme.png"
    "assets/open-in.png"
    "assets/option-bullet.png"
    "assets/qtbase_tr.qm"
    "assets/remove.png"
    "assets/scheme.png"
    "assets/spinner.gif"
    "assets/students.png"
    "assets/styles.qss"
    "assets/xls2csv.vbs"
)

qt6_add_resources(ikoOSKAR "resources"
    PREFIX
        "/"
    BASE
        "assets"
    FILES
        ${resources_resource_files}
)

set_target_properties(ikoOSKAR PROPERTIES
    WIN32_EXECUTABLE TRUE
)

install(TARGETS ikoOSKAR
    BUNDLE DESTINATION .
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)

if(QT_VERSION_MAJOR EQUAL 6)
    qt_finalize_executable(ikoOSKAR)
endif()
