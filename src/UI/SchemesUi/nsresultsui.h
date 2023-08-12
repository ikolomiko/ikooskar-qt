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
        explicit NSResultsUi(const QString& examName, const QString& examDate, QWidget* parent = nullptr);
        ~NSResultsUi();

    private:
        Ui::NSResultsUi* ui;
    };

} // namespace UI
} // namespace ikoOSKAR
#endif // IKOOSKAR_UI_NSRESULTSUI_H
