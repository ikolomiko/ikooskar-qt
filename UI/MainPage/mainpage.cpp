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
    db->Add(new Ogrenci{
                .OkulNo = 123,
                .Ad = "yeni deneme",
                .Soyad = "yeni soyad",
                .Sinif = 11,
                .Sube = "a"
    });

    //db->EndOfTheYear();


    /*db->Delete(new Ogrenci{
                   .Id = 12
               });*/
    QList<Ogrenci> ogrenciler = *db->GetAllStudents();

    /*
    Ogrenci *o2 = nullptr;
    for (int i = 0; i < ogrenciler.length(); i++){
        if (ogrenciler[i].Ad == "ad1"){
            o2 = &ogrenciler[i];
            break;
        }
    }
    o2->Ad = "ad5";
    db->Update(o2);
    */

    foreach (Ogrenci o, ogrenciler){
        qDebug() << o.Id << o.Ad
                 << o.Sinif;
    }

    //this->setWindowTitle(s);
    //this->setWindowTitle( db->DatabaseExists() );
    delete(db);
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
