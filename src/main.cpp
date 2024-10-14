#include "app.h"

#include <QApplication>

int main(int argc, char* argv[])
{
    QCoreApplication::setOrganizationName("ikoSoft");
    QCoreApplication::setApplicationName("ikoOSKAR");
    QCoreApplication::setApplicationVersion("4.2.0");

    ikoOSKAR::App app(argc, argv);
    return app.exec();
}
