#include "databaseui.h"
#include "ui_databaseui.h"

namespace ikoOSKAR {
namespace UI {

DatabaseUi::DatabaseUi(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::DatabaseUi)
{
    ui->setupUi(this);
    ui->tableWidget->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
}

DatabaseUi::~DatabaseUi()
{
    delete ui;
}

} // namespace UI
} // namespace ikoOSKAR
