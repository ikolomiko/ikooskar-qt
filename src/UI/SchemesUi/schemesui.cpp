#include "schemesui.h"
#include "examwidget.h"
#include "monthheaderwidget.h"
#include "newschemedialog.h"
#include "ui_schemesui.h"

namespace ikoOSKAR {
namespace UI {

    SchemesUi::SchemesUi(QWidget* parent)
        : Common::Module(parent)
        , ui(new Ui::SchemesUi)
    {
        ui->setupUi(this);
        name = new QString("Sınav Düzenleri");
        historyProvider = new BLL::HistoryProvider();

        auto history = historyProvider->getHistory();

        if (history.empty()) {
            return;
        }

        // History is not empty
        ui->historyRoot->layout()->removeWidget(ui->lblDescription);
        ui->lblDescription->deleteLater();

        for (auto it = history.end(); it != history.begin(); it--) {
            auto month = std::prev(it).key();
            const auto& examList = history[month];
            if (examList.empty()) {
                continue;
            }

            auto monthHeader = new MonthHeaderWidget(month);
            ui->historyRoot->layout()->addWidget(monthHeader);

            for (const auto& exam : examList) {
                auto examWidget = new ExamWidget(exam, 100);
                ui->historyRoot->layout()->addWidget(examWidget);
            }
        }
        ui->historyRoot->layout()->addItem(new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding));
    }

    SchemesUi::~SchemesUi()
    {
        delete ui;
    }

    SchemesUi* SchemesUi::getInstance()
    {
        if (instance == nullptr) {
            instance = new SchemesUi();
        }
        return instance;
    }

    const QString* SchemesUi::getDescription()
    {
        QString historyCount = QString::number(historyProvider->getHistoryCountForCurrentTerm());
        QString termString = historyProvider->getCurrentTermString();
        return new QString(termString + " döneminde toplam " + historyCount + " adet sınav düzeni oluşturdunuz");
    }

    void SchemesUi::on_btnNewScheme_clicked()
    {
        NewSchemeDialog dialog(this);
        if (dialog.exec() == QDialog::Accepted) {
            // refresh history ui
            // refresh description
            // if mode==demo, decrement remainings by one
        }
    }

} // namespace UI
} // namespace ikoOSKAR
