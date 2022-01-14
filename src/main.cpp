#include "UI/MainPage/mainpage.h"

#include <QApplication>

int main(int argc, char* argv[])
{
    QApplication a(argc, argv);
    ikoOSKAR::UI::MainPage w;
    w.show();
    return a.exec();
}
