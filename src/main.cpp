#include "UI/MainPage/mainpage.h"

#include <QApplication>

int main(int argc, char* argv[])
{
    QFile styles(":/styles.qss");
    styles.open(QFile::ReadOnly);

    QApplication a(argc, argv);
    a.setStyleSheet(QLatin1String(styles.readAll()));
    ikoOSKAR::UI::MainPage w;
    w.show();
    return a.exec();
}
