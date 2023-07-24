#include "schemesui.h"
#include "ui_schemesui.h"

namespace ikoOSKAR {
namespace UI {

    SchemesUi::SchemesUi(QWidget* parent)
        : Common::Module(parent)
        , ui(new Ui::SchemesUi)
    {
        ui->setupUi(this);
        name = new QString("Sınav Düzenleri");
    }

    SchemesUi::~SchemesUi()
    {
        delete ui;
    }

    SchemesUi* SchemesUi::getInstance()
    {
        if (instance == nullptr) {
            instance = new SchemesUi();
        }
        return instance;
    }

    const QString* SchemesUi::getDescription()
    {
        return new QString("2023-2024 döneminde toplam x adet sınav düzeni oluşturdunuz");
    }
} // namespace UI
} // namespace ikoOSKAR
