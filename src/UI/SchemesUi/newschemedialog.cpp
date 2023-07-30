#include "newschemedialog.h"
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

        nav->btnPrev->hide();
        page = EXAM_INFO;
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

        auto currentWidget = ui->root->widget(ui->root->currentIndex());
        ui->root->removeWidget(currentWidget);
        currentWidget->deleteLater();
        page = (PageState)((int)page + 1);
    }

} // namespace UI
} // namespace ikoOSKAR
