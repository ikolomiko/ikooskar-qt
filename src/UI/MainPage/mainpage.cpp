#include "mainpage.h"
#include "ui_mainpage.h"
#include <QDebug>

namespace ikoOSKAR {
namespace UI {
    enum MainPage::Subpage : int {
        HOME,
        DATABASE,
        NEW_SCHEME,
        HISTORY,
        ABOUT
    };

    MainPage::MainPage(QWidget* parent)
        : QMainWindow(parent)
        , ui(new Ui::MainPage)
    {
        ui->setupUi(this);

        buttons = new QPushButton* [] { ui->btnHome, ui->btnDatabase, ui->btnNewScheme, ui->btnHistory, ui->btnHelp };
        modules = new Common::Module* [] { WelcomeUi::getInstance(), DatabaseUi::getInstance(), SchemeGeneratorUi::getInstance(), HistoryUi::getInstance(), AboutUi::getInstance() };

        for (int i = 0; i < 5; i++) {
            modules[i]->setDescriptionLabel(ui->lblDescription);
            ui->stackedWidget->addWidget(modules[i]);
        }

        changePage(HOME);

#ifdef QT_DEBUG
        setWindowTitle(windowTitle() + " DEBUG");
#endif
    }

    MainPage::~MainPage()
    {
        delete[] buttons;
        delete[] modules;
        delete ui;
    }

    void MainPage::changePage(Subpage index)
    {
        Common::Module* module = modules[index];

        ui->lblModuleName->setText(*module->getName());
        ui->lblDescription->setText(*module->getDescription());
        ui->btnCurrentPage->setIcon(buttons[index]->icon());
        ui->stackedWidget->setCurrentWidget(modules[index]);
    }

    void MainPage::on_btnHome_clicked()
    {
        changePage(HOME);
    }

    void MainPage::on_btnDatabase_clicked()
    {
        changePage(DATABASE);
    }

    void MainPage::on_btnNewScheme_clicked()
    {
        changePage(NEW_SCHEME);
    }

    void MainPage::on_btnHistory_clicked()
    {
        changePage(HISTORY);
    }

    void MainPage::on_btnHelp_clicked()
    {
        changePage(ABOUT);
    }

} // namespace UI
} // namespace ikoOSKAR
