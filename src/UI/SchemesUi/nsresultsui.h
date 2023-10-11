#ifndef IKOOSKAR_UI_NSRESULTSUI_H
#define IKOOSKAR_UI_NSRESULTSUI_H

#include <QWidget>

namespace ikoOSKAR {
namespace UI {

    namespace Ui {
        class NSResultsUi;
    }

    class NSResultsUi : public QWidget {
        Q_OBJECT

    public:
        explicit NSResultsUi(const QString& examName, const QString& examDate, const QString& pathClassLists, const QString& pathHallLayouts, QWidget* parent = nullptr, bool isHistorical = false);
        ~NSResultsUi();

    private:
        Ui::NSResultsUi* ui;
    };

} // namespace UI
} // namespace ikoOSKAR
#endif // IKOOSKAR_UI_NSRESULTSUI_H
