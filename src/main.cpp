#include "UI/MainPage/mainpage.h"

#include <QApplication>

int main(int argc, char* argv[])
{
    QApplication app(argc, argv);
    ikoOSKAR::UI::MainPage w;

    QTranslator baseTranslator;
    if (baseTranslator.load(QLocale::Turkish, QLatin1String("qtbase"), QLatin1String("_"), QLatin1String(":/")))
        QCoreApplication::installTranslator(&baseTranslator);

    QFile styles(":/styles.qss");
    styles.open(QFile::ReadOnly);

    QApplication::setAttribute(Qt::AA_DisableWindowContextHelpButton);
    app.setStyleSheet(QLatin1String(styles.readAll()));
    w.show();
    return app.exec();
}
