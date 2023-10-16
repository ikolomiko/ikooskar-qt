#include "aboutpage.h"
#include "ui_aboutpage.h"

namespace ikoOSKAR {
namespace UI {

    AboutPage::AboutPage(QWidget* parent)
        : Common::Page(parent)
        , ui(new Ui::AboutPage)
    {
        ui->setupUi(this);
        name = new QString("Yardım ve İletişim");
    }

    AboutPage::~AboutPage()
    {
        delete ui;
    }

    AboutPage* AboutPage::getInstance()
    {
        if (instance == nullptr) {
            instance = new AboutPage();
        }
        return instance;
    }

    const QString* AboutPage::getDescription()
    {
        return new QString("Lisans bilgisi: lorem ipsum");
    }

} // namespace UI
} // namespace ikoOSKAR
