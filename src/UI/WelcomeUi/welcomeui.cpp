#include "welcomeui.h"
#include "ui_welcomeui.h"
#include <QDebug>

namespace ikoOSKAR {
namespace UI {
    WelcomeUi::WelcomeUi(QWidget* parent)
        : QWidget(parent)
        , ui(new Ui::WelcomeUi)
    {
        ui->setupUi(this);
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
} // namespace UI
} // namespace ikoOSKAR
