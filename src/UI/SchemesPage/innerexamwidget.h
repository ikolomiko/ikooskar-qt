#ifndef IKOOSKAR_UI_INNEREXAMWIDGET_H
#define IKOOSKAR_UI_INNEREXAMWIDGET_H

#include <QWidget>

namespace ikoOSKAR {
namespace UI {

    namespace Ui {
        class InnerExamWidget;
    }

    class InnerExamWidget : public QWidget {
        Q_OBJECT

    public:
        explicit InnerExamWidget(const QString& examName, const QString& examDate, const QString& pathClassLists, const QString& pathHallLayouts, QWidget* parent = nullptr, bool isHistorical = false);
        ~InnerExamWidget();

    private:
        Ui::InnerExamWidget* ui;
    };

} // namespace UI
} // namespace ikoOSKAR
#endif // IKOOSKAR_UI_INNEREXAMWIDGET_H
