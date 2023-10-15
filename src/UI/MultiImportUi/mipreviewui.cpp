#include "mipreviewui.h"
#include "UI/DatabasePage/databasepage.h"
#include "ui_mipreviewui.h"

namespace ikoOSKAR {
namespace UI {

    MIPreviewUi::MIPreviewUi(QList<Student*>* students, QWidget* parent)
        : QWidget(parent)
        , ui(new Ui::MIPreviewUi)
    {
        ui->setupUi(this);
        btnConfirm = ui->btnConfirm;
        btnPrev = ui->btnPrev;

        QString placeholder = ui->lblDescription->text();
        placeholder.replace("<n>", QString::number(students->size()));
        placeholder.replace("<g>", QString::number(students->at(0)->grade));
        placeholder.replace("<s>", students->at(0)->section);
        ui->lblDescription->setText(placeholder);

        ui->frame->layout()->addWidget(new DatabasePage::ClassTable(students));
    }

    MIPreviewUi::~MIPreviewUi()
    {
        delete ui;
    }

} // namespace UI
} // namespace ikoOSKAR
