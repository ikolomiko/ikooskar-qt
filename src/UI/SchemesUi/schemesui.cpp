#include "schemesui.h"
#include "ui_schemesui.h"
#include "newschemedialog.h"

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


    void SchemesUi::on_btnNewScheme_clicked()
    {
        auto dialog = new NewSchemeDialog(this);
        if (dialog->exec() == QDialog::Accepted) {
            // refresh history ui
            // if mode==demo, decrement remainings by one
        }

    }

} // namespace UI
} // namespace ikoOSKAR
