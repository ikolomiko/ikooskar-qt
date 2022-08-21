#include "aboutui.h"
#include "ui_aboutui.h"

namespace ikoOSKAR {
namespace UI {

    AboutUi::AboutUi(QWidget* parent)
        : Common::Module(parent)
        , ui(new Ui::AboutUi)
    {
        ui->setupUi(this);
        name = new QString("Yardım ve İletişim");
    }

    AboutUi::~AboutUi()
    {
        delete ui;
    }

    AboutUi* AboutUi::getInstance()
    {
        if (instance == nullptr) {
            instance = new AboutUi();
        }
        return instance;
    }

    const QString* AboutUi::getDescription()
    {
        return new QString("Lisans bilgisi: lorem ipsum");
    }

} // namespace UI
} // namespace ikoOSKAR
