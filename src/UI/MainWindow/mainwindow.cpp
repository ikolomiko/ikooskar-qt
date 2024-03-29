#include "mainwindow.h"
#include "BLL/UpgradeAssistant/upgradeassistant.h"
#include "UI/AboutPage/aboutpage.h"
#include "UI/Common/spinner.h"
#include "UI/DatabasePage/databasepage.h"
#include "UI/ErrorUi/errorui.h"
#include "UI/SchemesPage/schemespage.h"
#include "UI/WelcomePage/welcomepage.h"
#include "app.h"
#include "ui_mainwindow.h"
#include <QMessageBox>
#include <QStatusBar>
#include <QThreadPool>

namespace ikoOSKAR {
namespace UI {
    enum MainWindow::Subpage : int {
        HOME,
        DATABASE,
        SCHEMES,
        ABOUT
    };

    MainWindow::MainWindow(QWidget* parent)
        : QMainWindow(parent)
        , ui(new Ui::MainWindow)
        , authenticator(BLL::Authenticator::getInstance())
    {
        ui->setupUi(this);
        setWindowTitle(ikoOSKAR::App::applicationDisplayName());
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

            connect(assistant, &BLL::UpgradeAssistant::upgradeFinished, this, &MainWindow::handleFinishedUpgrade);
            connect(assistant, &BLL::UpgradeAssistant::error, [](const QString& message) {
                ErrorUi("Güncelleme Hatası").displayMessage(message);
            });
            QThreadPool::globalInstance()->start(assistant);
        } else {
            initSubpages();
        }
    }

    void MainWindow::initSubpages()
    {
        QPushButton* buttons[4] = { ui->btnHome, ui->btnDatabase, ui->btnSchemes, ui->btnHelp };
        Common::Page* pages[4] = { WelcomePage::getInstance(), DatabasePage::getInstance(), SchemesPage::getInstance(), AboutPage::getInstance() };

        for (int i = 0; i < 4; i++) {
            const auto& btn = buttons[i];
            connect(btn, &QPushButton::clicked, this, [=]() {
                changePage((Subpage)i, btn->icon());
            });
        }

        for (int i = 0; i < 4; i++) {
            ui->stackedWidget->addWidget(pages[i]);
            connect(pages[i], &Common::Page::descriptionUpdated, this, [=](const QString& description) {
                if (i == ui->stackedWidget->currentIndex()) {
                    setDescription(description);
                }
            });
        }

        changePage(HOME, buttons[HOME]->icon());

        refreshDemoStatus();
        connect(authenticator, &BLL::Authenticator::demoUpdated, this, &MainWindow::refreshDemoStatus);
    }

    MainWindow::~MainWindow()
    {
        delete ui;
    }

    void MainWindow::setDescription(const QString& description)
    {
        ui->lblDescription->setText(description);
    }

    void MainWindow::handleFinishedUpgrade(int nStudents)
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

    void MainWindow::changePage(Subpage index, QIcon icon)
    {
        auto page = (Common::Page*)ui->stackedWidget->widget(index);

        ui->lblPageName->setText(*page->getName());
        ui->lblDescription->setText(*page->getDescription());
        ui->btnCurrentPage->setIcon(icon);
        ui->stackedWidget->setCurrentIndex(index);
    }

    void MainWindow::refreshDemoStatus()
    {
        auto status = authenticator->getLicenseStatus();
        if (status == BLL::LicenseStatus::Activated) {
            return;
        } else if (status == BLL::LicenseStatus::Demo) {
            status.text += ". " + QString::number(authenticator->getDemoRemainings()) + " adet ücretsiz deneme hakkınız kaldı.";
        }

        statusBar()->showMessage(status.text);
        statusBar()->setStyleSheet(status.style);
    }

} // namespace UI
} // namespace ikoOSKAR
