#ifndef IKOOSKAR_UI_NEWSCHEMEDIALOG_H
#define IKOOSKAR_UI_NEWSCHEMEDIALOG_H

#include "BLL/Authenticator/authenticator.h"
#include "BLL/SchemeGenerator/schemegenerator.h"
#include "UI/Common/twobuttonnav.h"
#include <QDate>
#include <QDialog>

namespace ikoOSKAR {
namespace UI {
    namespace Ui {
        class NewSchemeDialog;
    }

    class NewSchemeDialog : public QDialog {
        Q_OBJECT

    public:
        explicit NewSchemeDialog(QWidget* parent = nullptr);
        ~NewSchemeDialog();

    private:
        enum PageState {
            EXAM_INFO = 0,
            CLASS_PICKER,
            HALL_PICKER,
            PREVIEW,
            SPINNER,
            RESULTS
        };

        Ui::NewSchemeDialog* ui;
        Common::TwoButtonNav* nav;
        BLL::SchemeGenerator* bll;
        PageState page;
        QString examName;
        QDate examDate;
        QString pathClassLists, pathHallLayouts;
        BLL::Authenticator* authenticator;

        void prevPage();
        void nextPage();
        void handleError(const QString& errorMessage);
        void onExportFinished(const QString& pathClassLists, const QString& pathHallLayouts);
    };
} // namespace UI
} // namespace ikoOSKAR
#endif // IKOOSKAR_UI_NEWSCHEMEDIALOG_H
