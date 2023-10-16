#include "welcomepage.h"
#include "ui_welcomepage.h"

namespace ikoOSKAR {
namespace UI {
    WelcomePage::WelcomePage(QWidget* parent)
        : Common::Page(parent)
        , ui(new Ui::WelcomePage)
    {
        ui->setupUi(this);
        name = new QString("Ana Sayfa");
    }

    WelcomePage::~WelcomePage()
    {
        delete ui;
    }

    WelcomePage* WelcomePage::getInstance()
    {
        if (instance == nullptr) {
            instance = new WelcomePage();
        }
        return instance;
    }

    const QString* WelcomePage::getDescription()
    {
        return new QString("Ana sayfadasınız");
    }
} // namespace UI
} // namespace ikoOSKAR
