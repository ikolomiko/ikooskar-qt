#include "databaseui.h"
#include "ui_databaseui.h"
#include <QDebug>

namespace ikoOSKAR {
namespace UI {

DatabaseUi::DatabaseUi(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::DatabaseUi)
{
    qDebug() << "Construct";
    ui->setupUi(this);
    ui->tableWidget->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
}

DatabaseUi::~DatabaseUi()
{
    qDebug() << "Killed";
    delete ui;
}

} // namespace UI
} // namespace ikoOSKAR
