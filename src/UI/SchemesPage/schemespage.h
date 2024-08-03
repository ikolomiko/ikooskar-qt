#ifndef IKOOSKAR_UI_SCHEMESPAGE_H
#define IKOOSKAR_UI_SCHEMESPAGE_H

#include "BLL/Authenticator/authenticator.h"
#include "BLL/HistoryProvider/historyprovider.h"
#include "UI/Common/page.h"
#include <QLayoutItem>
#include <QScrollArea>
#include <QWidget>

namespace ikoOSKAR {
namespace UI {

    namespace Ui {
        class SchemesPage;
    }

    class SchemesPage : public Common::Page {
        Q_OBJECT

    public:
        static SchemesPage* getInstance();
        const QString* getName();
        const QString* getDescription();
        ~SchemesPage();

    private slots:
        void on_btnNewScheme_clicked();

    private:
        BLL::HistoryProvider* historyProvider;
        Ui::SchemesPage* ui;
        inline static SchemesPage* instance;
        QList<QWidget*> historyWidgets;
        QLayoutItem* verticalSpacer;
        QLabel* lblEmptyHistory;
        BLL::Authenticator* authenticator;
        QWidget* responsiveWrap(QWidget*);

        explicit SchemesPage(QWidget* parent = nullptr);
        void setupHistoryUi(const History& history);
        void refreshHistory();
    };

} // namespace UI
} // namespace ikoOSKAR
#endif // IKOOSKAR_UI_SCHEMESPAGE_H
