#include "app.h"

#include <QApplication>
#include <QSettings>
#include <QStandardPaths>

int main(int argc, char* argv[])
{
    QCoreApplication::setOrganizationName("ikoSoft");
    QCoreApplication::setApplicationName("ikoOSKAR");
    QCoreApplication::setApplicationVersion("4.0.0-alpha.2");

    QSettings settings;
    settings.setValue("PATH_DOCS_ROOT", QStandardPaths::writableLocation(QStandardPaths::DocumentsLocation) + "/ikoOSKAR");
    settings.setValue("PRETTY_NAME", "iko Ortak Sınav Karma Sistemi");

    ikoOSKAR::App app(argc, argv);
    return app.exec();
}
