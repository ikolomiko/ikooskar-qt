#include "schemespage.h"
#include "UI/NewSchemeDialog/newschemedialog.h"
#include "examwidget.h"
#include "monthheaderwidget.h"
#include "ui_schemespage.h"

namespace ikoOSKAR {
namespace UI {

    SchemesPage::SchemesPage(QWidget* parent)
        : Common::Page(parent)
        , ui(new Ui::SchemesPage)
    {
        ui->setupUi(this);
        name = new QString("Sınav Düzenleri");
        historyProvider = new BLL::HistoryProvider();

        lblEmptyHistory = new QLabel("Daha önce oluşturulmuş sınav düzeni bulunamadı");
        lblEmptyHistory->setAlignment(Qt::AlignCenter);
        lblEmptyHistory->setObjectName("lblDescription");

        verticalSpacer = nullptr;

        setupHistoryUi();
    }

    void SchemesPage::setupHistoryUi()
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
        auto history = historyProvider->getHistory();
        if (history.empty()) {
            ui->historyRoot->layout()->addWidget(lblEmptyHistory);
            return;
        }

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

        historyProvider->refresh();
        setupHistoryUi();
        emit descriptionUpdated(*getDescription());
    }

} // namespace UI
} // namespace ikoOSKAR
