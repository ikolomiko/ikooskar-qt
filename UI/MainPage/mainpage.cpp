#include "mainpage.h"
#include "ui_mainpage.h"

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

} // namespace UI
} // namespace ikoOSKAR
