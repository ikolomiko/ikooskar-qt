#include "newschemedialog.h"
#include "UI/ErrorUi/errorui.h"
#include "nsexaminfoui.h"
#include "ui_newschemedialog.h"

namespace ikoOSKAR {
namespace UI {

    NewSchemeDialog::NewSchemeDialog(QWidget* parent)
        : QDialog(parent)
        , ui(new Ui::NewSchemeDialog)
        , nav(new Common::TwoButtonNav)
    {
        ui->setupUi(this);
        ui->verticalLayout->addWidget(nav);

        connect(nav->btnPrev, &QPushButton::clicked, this, &NewSchemeDialog::prevPage);
        connect(nav->btnNext, &QPushButton::clicked, this, &NewSchemeDialog::nextPage);

        nav->btnPrev->hide();
        page = EXAM_INFO;
        auto examInfo = new NSExamInfoUi();
        ui->root->addWidget(examInfo);
    }

    NewSchemeDialog::~NewSchemeDialog()
    {
        delete ui;
    }

    void NewSchemeDialog::prevPage()
    {
        switch (page) {
        case EXAM_INFO: {
            // Illegal state, should never happen
            return;
        }
        case CLASS_PICKER: {
            nav->btnPrev->hide();
            break;
        }
        case HALL_PICKER: {
            break;
        }
        case PREVIEW: {
            break;
        }
        case SPINNER: {
            // Illegal state, there's no going back from the spinner page
            return;
        }
        case RESULTS: {
            // Illegal state, there's no going back from the results page
            return;
        }
        }

        auto currentWidget = ui->root->widget(ui->root->currentIndex());
        ui->root->removeWidget(currentWidget);
        currentWidget->deleteLater();
        page = (PageState)((int)page - 1);
    }

    void NewSchemeDialog::nextPage()
    {
        switch (page) {
        case EXAM_INFO: {
            auto examInfo = (NSExamInfoUi*)ui->root->currentWidget();
            examName = examInfo->getExamName();
            examDate = examInfo->getExamDate();

            // Init bll
            // setExamName
            // setExamDate
            // check for name and name-date
            // return on any error
            // continue if all checks passed

            nav->btnPrev->show();
            break;
        }
        case CLASS_PICKER: {
            break;
        }
        case HALL_PICKER: {
            break;
        }
        case PREVIEW: {
            break;
        }
        case SPINNER: {
            break;
        }
        case RESULTS: {
            // Close the NewSchemeDialog with the accepted signal
            accept();
            return;
        }
        }
        page = (PageState)((int)page + 1);
    }

    void NewSchemeDialog::handleError(const QString& errorMessage)
    {
        ErrorUi("HATA").displayMessage(errorMessage);
    }

} // namespace UI
} // namespace ikoOSKAR
