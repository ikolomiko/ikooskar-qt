#include "mainpage.h"
#include "ui_mainpage.h"
#include "DAL/database.h"

MainPage::MainPage(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainPage)
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
    QString s = "a";
    ikoOSKAR::DAL::Database db(&s);

    this->setWindowTitle(s);
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

