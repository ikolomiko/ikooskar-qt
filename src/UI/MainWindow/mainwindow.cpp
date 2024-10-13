#include "mainwindow.h"
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

        initSubpages();
    }

    void MainWindow::initSubpages()
    {
        QPushButton* buttons[4] = { ui->btnHome, ui->btnDatabase, ui->btnSchemes, ui->btnHelp };
        Common::Page* pages[4] = { WelcomePage::getInstance(), DatabasePage::getInstance(), SchemesPage::getInstance(), AboutPage::getInstance() };

        for (int i = 0; i < 4; i++) {
            const auto& btn = buttons[i];
            connect(btn, &QPushButton::clicked, this, [btn, i, this]() {
                changePage((Subpage)i, btn->icon());
            });
        }

        for (int i = 0; i < 4; i++) {
            ui->stackedWidget->addWidget(pages[i]);
            connect(pages[i], &Common::Page::descriptionUpdated, this, [this, i](const QString& description) {
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
