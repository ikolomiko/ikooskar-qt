#include "mainpage.h"
#include "UI/AboutUi/aboutui.h"
#include "UI/DatabaseUi/databaseui.h"
#include "UI/SchemesUi/schemesui.h"
#include "UI/WelcomeUi/welcomeui.h"
#include "ui_mainpage.h"
#include <QDebug>
#include <QStatusBar>

namespace ikoOSKAR {
namespace UI {
    enum MainPage::Subpage : int {
        HOME,
        DATABASE,
        SCHEMES,
        ABOUT
    };

    MainPage::MainPage(QWidget* parent)
        : QMainWindow(parent)
        , ui(new Ui::MainPage)
    {
        ui->setupUi(this);

        QPushButton* buttons[4] = { ui->btnHome, ui->btnDatabase, ui->btnSchemes, ui->btnHelp };
        Common::Module* modules[4] = { WelcomeUi::getInstance(), DatabaseUi::getInstance(), SchemesUi::getInstance(), AboutUi::getInstance() };

        for (int i = 0; i < 4; i++) {
            const auto& btn = buttons[i];
            connect(btn, &QPushButton::clicked, this, [=]() {
                changePage((Subpage)i, btn->icon());
            });
        }

        for (int i = 0; i < 4; i++) {
            ui->stackedWidget->addWidget(modules[i]);
            connect(modules[i], &Common::Module::descriptionUpdated, this, &MainPage::setDescription);
        }

        changePage(HOME, buttons[HOME]->icon());

#ifdef QT_DEBUG
        setWindowTitle(windowTitle() + " DEBUG");
#endif
    }

    MainPage::~MainPage()
    {
        delete ui;
    }

    void MainPage::setDescription(const QString& description)
    {
        ui->lblDescription->setText(description);
    }

    void MainPage::changePage(Subpage index, QIcon icon)
    {
        auto module = (Common::Module*)ui->stackedWidget->widget(index);

        ui->lblModuleName->setText(*module->getName());
        ui->lblDescription->setText(*module->getDescription());
        ui->btnCurrentPage->setIcon(icon);
        ui->stackedWidget->setCurrentIndex(index);
    }

} // namespace UI
} // namespace ikoOSKAR
