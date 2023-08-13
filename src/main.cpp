#include "UI/MainPage/mainpage.h"

#include <QApplication>
#include <QFile>
#include <QSettings>
#include <QStandardPaths>
#include <QTranslator>

int main(int argc, char* argv[])
{
    QCoreApplication::setOrganizationName("ikoSoft");
    QCoreApplication::setApplicationName("ikoOSKAR");

    QSettings().setValue("PATH_DOCS_ROOT", QStandardPaths::writableLocation(QStandardPaths::DocumentsLocation) + "/ikoOSKAR");

    QApplication app(argc, argv);
    ikoOSKAR::UI::MainPage w;

    QTranslator baseTranslator;
    if (baseTranslator.load(QLocale::Turkish, QLatin1String("qtbase"), QLatin1String("_"), QLatin1String(":/")))
        QCoreApplication::installTranslator(&baseTranslator);

    QFile styles(":/styles.qss");
    styles.open(QFile::ReadOnly);

    app.setStyleSheet(QLatin1String(styles.readAll()));
    w.show();
    return app.exec();
}
