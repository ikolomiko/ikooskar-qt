#include "newschemedialog.h"
#include "BLL/SchemeExporter/schemeexporter.h"
#include "UI/Common/spinner.h"
#include "UI/ErrorUi/errorui.h"
#include "nsclasspickerui.h"
#include "nsexaminfoui.h"
#include "nshallpickerui.h"
#include "nspreviewui.h"
#include "ui_newschemedialog.h"
#include <QtConcurrent/QtConcurrentRun>

namespace ikoOSKAR {
namespace UI {

    NewSchemeDialog::NewSchemeDialog(QWidget* parent)
        : QDialog(parent)
        , ui(new Ui::NewSchemeDialog)
        , nav(new Common::TwoButtonNav)
        , bll(new BLL::SchemeGenerator)
    {
        ui->setupUi(this);
        ui->verticalLayout->addWidget(nav);

        connect(bll, &BLL::SchemeGenerator::error, this, &NewSchemeDialog::handleError);

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
        delete bll;
        delete nav;
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
            nav->btnNext->setText("  İleri  ");
            nav->btnNext->setIcon(QIcon(":/arrow-right.png"));
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
            examName = examInfo->getExamName().simplified();
            examDate = examInfo->getExamDate();

            bll->setDate(examDate);
            bool success = bll->setName(examName);
            if (!success) {
                // Necessary error mesage(s) have already been shown
                return;
            }

            nav->btnPrev->show();
            auto classPicker = new NSClassPickerUi();
            ui->root->addWidget(classPicker);
            ui->root->setCurrentIndex(ui->root->currentIndex() + 1);
            break;
        }
        case CLASS_PICKER: {
            auto classPicker = (NSClassPickerUi*)ui->root->currentWidget();
            auto attendingClassNames = classPicker->getAttendingClasses();

            bool success = bll->setAttendingClasses(attendingClassNames);
            if (!success) {
                // Necessary error mesage(s) have already been shown
                return;
            }

            auto hallPicker = new NSHallPickerUi();
            ui->root->addWidget(hallPicker);
            ui->root->setCurrentIndex(ui->root->currentIndex() + 1);
            break;
        }
        case HALL_PICKER: {
            auto hallPicker = (NSHallPickerUi*)ui->root->currentWidget();
            auto selectedHallNames = hallPicker->getSelectedHalls();

            bool success = bll->setExamHalls(selectedHallNames);
            if (!success) {
                // Necessary error mesage(s) have already been shown
                return;
            }

            auto preview = new NSPreviewUi(bll->preview());
            ui->root->addWidget(preview);
            ui->root->setCurrentIndex(ui->root->currentIndex() + 1);
            nav->btnNext->setText("  Oluştur  ");
            nav->btnNext->setIcon(QIcon(":/check.png"));
            break;
        }
        case PREVIEW: {
            auto spinner = new Common::Spinner();
            spinner->setTitle("Sınav düzeni oluşturuluyor. Lütfen bekleyiniz.");
            spinner->start();
            ui->root->addWidget(spinner);
            ui->root->setCurrentIndex(ui->root->currentIndex() + 1);
            nav->hide();

            // generate and export scheme in another thread
            QtConcurrent::run([&]() {
                return bll->generate();
            }).then([&](Shared::Scheme scheme) {
                BLL::SchemeExporter exporter(scheme);
                connect(&exporter, &BLL::SchemeExporter::exportFinished, this, &UI::NewSchemeDialog::onExportFinished);
                exporter.exportAll();
            });

            // TODO // if demo, decrease demo.remainingBalance by one
            break;
        }
        case SPINNER: {
            // the scheme has been generated and exported as two xlsx files
            auto spinner = (Common::Spinner*)ui->root->widget(ui->root->currentIndex());
            ui->root->removeWidget(spinner);
            spinner->stop();
            spinner->deleteLater();

            // auto results = new NSResultsUi(pathClassLists, pathHallLayouts)
            // ui->root->addWidget(results);
            // ui->root->setCurrentIndex(ui->root->currentIndex() + 1);
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

    void NewSchemeDialog::onExportFinished(const QString& pathClassLists, const QString& pathHallLayouts)
    {
        this->pathClassLists = pathClassLists;
        this->pathHallLayouts = pathHallLayouts;
        nextPage(); // spinner -> results
    }

} // namespace UI
} // namespace ikoOSKAR
