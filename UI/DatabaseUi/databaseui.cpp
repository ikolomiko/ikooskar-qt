#include "databaseui.h"
#include "ui_databaseui.h"

#include "UI/ErrorUi/errorui.h"

namespace ikoOSKAR {
namespace UI {

    DatabaseUi::DatabaseUi(QWidget* parent)
        : QWidget(parent)
        , ui(new Ui::DatabaseUi)
    {
        ui->setupUi(this);
        ui->tableWidget->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);

        const QString* errorTitle = new QString("Öğrenci İşlemlerinde Hata Oluştu");
        bll = new BLL::DatabaseHelper(new ErrorUi(*errorTitle));
    }

    DatabaseUi::~DatabaseUi()
    {
        delete ui;
        delete bll;
    }

    void DatabaseUi::on_pushButton_clicked()
    {
        // Debug code here
        /*
        bll->Add(new Student{
                    .id = 1235324234,
                    .firstName = "iko",
                    .lastName = "ğ",
                    .grade = 12,
                    .section = "D"
                 });
        */

        bll->Delete(0);

        /*
        bll->Update(new Student{
                        .id =  1235324237,
                        .firstName = "UwU",
                        .lastName = "OwO",
                    });
        */
        /*
        bll->Delete(new Student{.id = 1235324235});
        */

        /*
        QString st = "";
        foreach(int i, bll->GetAllIds()){
            st += QString().number(i) + " ";
        }
        ErrorUi(bll->IdExists(1235324235) ? "true" : "false", st);
        */
    }

} // namespace UI
} // namespace ikoOSKAR
