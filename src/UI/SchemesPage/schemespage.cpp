#include "schemespage.h"
#include "UI/Common/spinner.h"
#include "UI/ErrorUi/errorui.h"
#include "UI/NewSchemeDialog/newschemedialog.h"
#include "examwidget.h"
#include "monthheaderwidget.h"
#include "ui_schemespage.h"

namespace ikoOSKAR {
namespace UI {

    SchemesPage::SchemesPage(QWidget* parent)
        : Common::Page(parent)
        , ui(new Ui::SchemesPage)
        , authenticator(BLL::Authenticator::getInstance())
    {
        ui->setupUi(this);
        name = new QString("Sınav Düzenleri");

        historyProvider = new BLL::HistoryProvider();
        connect(historyProvider, &BLL::HistoryProvider::historyReady, this, &SchemesPage::setupHistoryUi);
        refreshHistory();
        
        lblEmptyHistory = new QLabel("Daha önce oluşturulmuş sınav düzeni bulunamadı");
        lblEmptyHistory->setAlignment(Qt::AlignCenter);
        lblEmptyHistory->setObjectName("lblDescription");

        verticalSpacer = nullptr;
    }

    void SchemesPage::setupHistoryUi(const History& history)
    {
        // Clear the history root
        for (const auto& widget : historyWidgets) {
            ui->historyRoot->layout()->removeWidget(widget);
            widget->deleteLater();
        }
        historyWidgets.clear();
        if (verticalSpacer != nullptr) {
            ui->historyRoot->layout()->removeItem(verticalSpacer);
            delete verticalSpacer;
            verticalSpacer = nullptr;
        }

        // Show label if history is empty
        if (history.empty()) {
            ui->historyRoot->layout()->addWidget(lblEmptyHistory);
            lblEmptyHistory->show();
            return;
        }

        ui->historyRoot->layout()->removeWidget(lblEmptyHistory);
        lblEmptyHistory->hide();

        // History is not empty
        for (auto it = history.end(); it != history.begin(); it--) {
            auto month = std::prev(it).key();
            const auto& examList = history[month];
            if (examList.empty()) {
                continue;
            }

            auto monthHeader = new MonthHeaderWidget(month);
            ui->historyRoot->layout()->addWidget(monthHeader);
            historyWidgets.append(monthHeader);

            for (const auto& exam : examList) {
                auto examWidget = new ExamWidget(exam, 100);
                ui->historyRoot->layout()->addWidget(examWidget);
                historyWidgets.append(examWidget);
            }
        }
        if (verticalSpacer == nullptr) {
            verticalSpacer = new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding);
        }
        ui->historyRoot->layout()->addItem(verticalSpacer);

        emit descriptionUpdated(*getDescription());
    }

    void SchemesPage::refreshHistory()
    {
        // Clear the history root
        for (const auto& widget : historyWidgets) {
            ui->historyRoot->layout()->removeWidget(widget);
            widget->deleteLater();
        }
        historyWidgets.clear();

        auto spinner = new Common::Spinner();
        spinner->setTitle("Önceki sınav düzenleri yükleniyor");
        historyWidgets.append(spinner);
        ui->historyRoot->layout()->addWidget(spinner);
        spinner->start();

        historyProvider->refresh();
    }

    SchemesPage::~SchemesPage()
    {
        delete ui;
    }

    SchemesPage* SchemesPage::getInstance()
    {
        if (instance == nullptr) {
            instance = new SchemesPage();
        }
        return instance;
    }

    const QString* SchemesPage::getDescription()
    {
        QString historyCount = QString::number(historyProvider->getHistoryCountForCurrentTerm());
        QString termString = historyProvider->getCurrentTermString();
        return new QString(termString + " döneminde toplam " + historyCount + " adet sınav düzeni oluşturdunuz");
    }

    void SchemesPage::on_btnNewScheme_clicked()
    {
        NewSchemeDialog dialog(this);
        dialog.exec();

        refreshHistory();
        if (authenticator->getLicenseStatus() == BLL::LicenseStatus::EndOfDemo) {
            ui->btnNewScheme->setEnabled(false);
            ErrorUi("Deneme Haklarınız Tükendi")
                .displayMessage("Ücretsiz deneme haklarınız tükenmiştir. Programı kullanmaya"
                                " devam etmek istiyorsanız lütfen iletisim@ikooskar.com.tr e-posta"
                                " adresinden iletişime geçerek bir lisans anahtarı satın alınız.");
        }
    }

} // namespace UI
} // namespace ikoOSKAR
