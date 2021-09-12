#include "mainpage.h"
#include "ui_mainpage.h"
#include "DAL/database.h"
#include <QList>
#include <QDebug>
using namespace ikoOSKAR::DAL;
namespace ikoOSKAR {
namespace UI {

MainPage::MainPage(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainPage)
{
    ui->setupUi(this);
}

MainPage::~MainPage()
{
    delete ui;
}

void MainPage::on_btn_database_clicked()
{
    //this->setWindowTitle("Öğrenci işlemleri açılıyor");
    Database* db = new Database();
    QList<Ogrenci> ogrenciler = *db->GetAllStudents();

    foreach (Ogrenci o, ogrenciler){
        qDebug() << o.Ad;
    }

    //this->setWindowTitle(s);
    //this->setWindowTitle( db->DatabaseExists() );

}


void MainPage::on_btn_newscheme_clicked()
{
    setWindowTitle("Yeni sınav düzeni oluşturuluyor...");
}


void MainPage::on_btn_history_clicked()
{
    setWindowTitle("Önceki sınav düzenleri görüntüleniyor");
}


void MainPage::on_btn_help_clicked()
{
    setWindowTitle("Yardııııımmmmm");
}

} // namespace UI
} // namespace ikoOSKAR
