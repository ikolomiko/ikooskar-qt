#ifndef IKOOSKAR_UI_NSEXAMINFOUI_H
#define IKOOSKAR_UI_NSEXAMINFOUI_H

#include <QWidget>

namespace ikoOSKAR {
namespace UI {

    namespace Ui {
        class NSExamInfoUi;
    }

    class NSExamInfoUi : public QWidget {
        Q_OBJECT

    public:
        explicit NSExamInfoUi(QWidget* parent = nullptr);
        ~NSExamInfoUi();
        QString getExamName();
        QDate getExamDate();

    private:
        Ui::NSExamInfoUi* ui;
    };

} // namespace UI
} // namespace ikoOSKAR
#endif // IKOOSKAR_UI_NSEXAMINFOUI_H
