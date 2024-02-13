#include "app.h"

#include <QApplication>

int main(int argc, char* argv[])
{
    QCoreApplication::setOrganizationName("ikoSoft");
    QCoreApplication::setApplicationName("ikoOSKAR");
    QCoreApplication::setApplicationVersion("4.0.0-alpha.2");

    ikoOSKAR::App app(argc, argv);
    return app.exec();
}
