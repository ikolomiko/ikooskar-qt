#include "welcomeui.h"
#include "ui_welcomeui.h"

namespace ikoOSKAR {
namespace UI {
    WelcomeUi::WelcomeUi(QWidget* parent)
        : Common::Module(parent)
        , ui(new Ui::WelcomeUi)
    {
        ui->setupUi(this);
        name = new QString("Ana Sayfa");
    }

    WelcomeUi::~WelcomeUi()
    {
        delete ui;
    }

    WelcomeUi* WelcomeUi::getInstance()
    {
        if (instance == nullptr) {
            instance = new WelcomeUi();
        }
        return instance;
    }

    const QString* WelcomeUi::getDescription()
    {
        return new QString("Ana sayfadasınız");
    }
} // namespace UI
} // namespace ikoOSKAR
