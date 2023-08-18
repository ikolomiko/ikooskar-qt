#include "mainpage.h"
#include "BLL/UpgradeAssistant/upgradeassistant.h"
#include "UI/AboutUi/aboutui.h"
#include "UI/Common/spinner.h"
#include "UI/DatabaseUi/databaseui.h"
#include "UI/ErrorUi/errorui.h"
#include "UI/SchemesUi/schemesui.h"
#include "UI/WelcomeUi/welcomeui.h"
#include "ui_mainpage.h"
#include <QMessageBox>
#include <QThreadPool>

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

#ifdef QT_DEBUG
        setWindowTitle(windowTitle() + " DEBUG");
#endif

        auto assistant = new BLL::UpgradeAssistant();
        if (assistant->willUpgrade()) {
            auto* spinner = new Common::Spinner();
            spinner->setTitle("Güncelleme yapılıyor...");
            spinner->start();
            ui->frameHeaderbar->hide();
            ui->stackedWidget->addWidget(spinner);

            connect(assistant, &BLL::UpgradeAssistant::upgradeFinished, this, &MainPage::handleFinishedUpgrade);
            QThreadPool::globalInstance()->start(assistant);
        } else {
            initSubpages();
        }
    }

    void MainPage::initSubpages()
    {
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
    }

    MainPage::~MainPage()
    {
        delete ui;
    }

    void MainPage::setDescription(const QString& description)
    {
        ui->lblDescription->setText(description);
    }

    void MainPage::handleFinishedUpgrade(int nStudents)
    {
        auto* spinner = (Common::Spinner*)ui->stackedWidget->currentWidget();
        ui->stackedWidget->removeWidget(spinner);
        spinner->deleteLater();
        ui->frameHeaderbar->show();
        initSubpages();
        if (nStudents < 0) {
            ErrorUi("Güncelleme Hatası").displayMessage("Bir hata oluştu, ikoOSKAR'ın eski sürümündeki öğrenci bilgileri aktarılamadı.");
        } else {
            auto text = QString("Güncelleme başarılı, ikoOSKAR'ın eski sürümündeki %1 adet öğrenci kaydı başarıyla içeri aktarıldı.").arg(nStudents);
            QMessageBox::information(this, "Güncelleme Başarılı", text);
        }
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
