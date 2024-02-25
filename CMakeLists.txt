cmake_minimum_required(VERSION 3.16)
project(ikoOSKAR VERSION 1.0 LANGUAGES CXX C)

set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_CXX_STANDARD 17)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
#set(CMAKE_AUTORCC ON)

find_package(QT NAMES Qt6 REQUIRED COMPONENTS Widgets)
find_package(Qt6 REQUIRED COMPONENTS Widgets Sql Network)
find_package(EXPAT REQUIRED)

set(PROJECT_SOURCES
    assets/icon.rc
    src/app.h src/app.cpp
    src/BLL/Authenticator/authenticator.h src/BLL/Authenticator/authenticator.cpp
    src/BLL/Authenticator/licensestatus.h src/BLL/Authenticator/licensestatus.cpp
    src/BLL/DatabaseHelper/databasehelper.cpp src/BLL/DatabaseHelper/databasehelper.h
    src/BLL/HistoryProvider/historyprovider.h src/BLL/HistoryProvider/historyprovider.cpp
    src/BLL/MultiImportHelper/multiimporthelper.h src/BLL/MultiImportHelper/multiimporthelper.cpp
    src/BLL/StudentEditor/studenteditor.cpp src/BLL/StudentEditor/studenteditor.h
    src/BLL/SchemeExporter/schemeexporter.h src/BLL/SchemeExporter/schemeexporter.cpp
    src/BLL/SchemeGenerator/schemegenerator.h src/BLL/SchemeGenerator/schemegenerator.cpp
    src/BLL/UpgradeAssistant/upgradeassistant.h src/BLL/UpgradeAssistant/upgradeassistant.cpp
    src/DAL/CloudAuth/cloudauth.h src/DAL/CloudAuth/cloudauth.cpp
    src/DAL/CloudAuth/cloudauthresponse.h
    src/DAL/Database/database.cpp src/DAL/Database/database.h
    src/DAL/LocalAuth/localauth.h src/DAL/LocalAuth/localauth.cpp
    src/DAL/MultiImport/multiimport.h src/DAL/MultiImport/multiimport.cpp
    src/Shared/desk.h
    src/Shared/deskcoordinates.h
    src/Shared/examstudent.h
    src/Shared/hall.h src/Shared/hall.cpp
    src/Shared/pattern.h src/Shared/pattern.cpp
    src/Shared/scheme.h
    src/Shared/student.h
    src/UI/AboutPage/aboutpage.cpp src/UI/AboutPage/aboutpage.h src/UI/AboutPage/aboutpage.ui
    src/UI/AuthenticatorWindow/authenticatorwindow.h src/UI/AuthenticatorWindow/authenticatorwindow.cpp src/UI/AuthenticatorWindow/authenticatorwindow.ui
    src/UI/Common/contactwidget.h src/UI/Common/contactwidget.cpp src/UI/Common/contactwidget.ui
    src/UI/Common/page.h
    src/UI/Common/spinner.h src/UI/Common/spinner.cpp src/UI/Common/spinner.ui
    src/UI/Common/twobuttonnav.h src/UI/Common/twobuttonnav.cpp
    src/UI/DatabasePage/databasepage.cpp src/UI/DatabasePage/databasepage.h src/UI/DatabasePage/databasepage.ui
    src/UI/ErrorUi/errorui.cpp src/UI/ErrorUi/errorui.h
    src/UI/MainWindow/mainwindow.cpp src/UI/MainWindow/mainwindow.h src/UI/MainWindow/mainwindow.ui
    src/UI/MultiImportDialog/multiimportdialog.cpp src/UI/MultiImportDialog/multiimportdialog.h src/UI/MultiImportDialog/multiimportdialog.ui
    src/UI/MultiImportDialog/miclasspickerui.h src/UI/MultiImportDialog/miclasspickerui.cpp src/UI/MultiImportDialog/miclasspickerui.ui
    src/UI/MultiImportDialog/mifilepickerui.h src/UI/MultiImportDialog/mifilepickerui.cpp src/UI/MultiImportDialog/mifilepickerui.ui
    src/UI/MultiImportDialog/mipreviewui.h src/UI/MultiImportDialog/mipreviewui.cpp src/UI/MultiImportDialog/mipreviewui.ui
    src/UI/NewSchemeDialog/newschemedialog.h src/UI/NewSchemeDialog/newschemedialog.cpp src/UI/NewSchemeDialog/newschemedialog.ui
    src/UI/NewSchemeDialog/nsclasspickerui.h src/UI/NewSchemeDialog/nsclasspickerui.cpp src/UI/NewSchemeDialog/nsclasspickerui.ui
    src/UI/NewSchemeDialog/nsexaminfoui.h src/UI/NewSchemeDialog/nsexaminfoui.cpp src/UI/NewSchemeDialog/nsexaminfoui.ui
    src/UI/NewSchemeDialog/nshallpickerui.h src/UI/NewSchemeDialog/nshallpickerui.cpp src/UI/NewSchemeDialog/nshallpickerui.ui
    src/UI/NewSchemeDialog/nspreviewui.h src/UI/NewSchemeDialog/nspreviewui.cpp src/UI/NewSchemeDialog/nspreviewui.ui
    src/UI/SchemesPage/innerexamwidget.h src/UI/SchemesPage/innerexamwidget.cpp src/UI/SchemesPage/innerexamwidget.ui
    src/UI/SchemesPage/monthheaderwidget.h src/UI/SchemesPage/monthheaderwidget.cpp
    src/UI/SchemesPage/schemespage.cpp src/UI/SchemesPage/schemespage.h src/UI/SchemesPage/schemespage.ui
    src/UI/StudentEditorDialog/studenteditordialog.cpp src/UI/StudentEditorDialog/studenteditordialog.h src/UI/StudentEditorDialog/studenteditordialog.ui
    src/UI/WelcomePage/welcomepage.cpp src/UI/WelcomePage/welcomepage.h src/UI/WelcomePage/welcomepage.ui    
    src/UI/SchemesPage/xlsxbutton.h src/UI/SchemesPage/xlsxbutton.cpp src/UI/SchemesPage/xlsxbutton.ui
    src/UI/SchemesPage/examwidget.h src/UI/SchemesPage/examwidget.cpp
    src/main.cpp
    deps/libxlsxwriter/third_party/minizip/ioapi.c deps/libxlsxwriter/third_party/minizip/unzip.c
    deps/freexl/freexl.c deps/freexl/freexl_xlsx.c
)

include_directories("src")
include_directories("deps/libxlsxwriter")

add_subdirectory("deps/libxlsxwriter" EXCLUDE_FROM_ALL)

qt_add_executable(ikoOSKAR
    MANUAL_FINALIZATION
    ${PROJECT_SOURCES}
)

target_link_libraries(ikoOSKAR PRIVATE
    Qt6::Widgets
    Qt6::Core
    Qt6::Gui
    Qt6::Sql
    Qt6::Network
    xlsxwriter
    ${EXPAT_LIBRARIES}
)

target_compile_options(ikoOSKAR PRIVATE
    $<$<COMPILE_LANGUAGE:CXX>:
    -Wall
    -Werror
    -Wextra>
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
    "assets/email.png"
    "assets/folder.png"
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
    "assets/splash.png"
    "assets/students.png"
    "assets/styles.qss"
    "assets/web.png"
    "assets/xlsx.png"
    "assets/NotoSans.ttf"
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

if (UNIX)
    install(DIRECTORY share/ DESTINATION share)
endif()

if(QT_VERSION_MAJOR EQUAL 6)
    qt_finalize_executable(ikoOSKAR)
endif()
