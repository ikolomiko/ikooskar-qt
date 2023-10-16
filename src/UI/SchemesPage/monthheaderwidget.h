#ifndef IKOOSKAR_UI_MONTHHEADERWIDGET_H
#define IKOOSKAR_UI_MONTHHEADERWIDGET_H

#include <QDate>
#include <QFrame>
#include <QHBoxLayout>
#include <QLabel>
#include <QWidget>

namespace ikoOSKAR {
namespace UI {

    class MonthHeaderWidget : public QWidget {
    private:
        QFrame* headerLine;
        QLabel* label;
        QHBoxLayout* layout;

    public:
        MonthHeaderWidget(QDate date);
    };

} // namespace UI
} // namespace ikoOSKAR

#endif // IKOOSKAR_UI_MONTHHEADERWIDGET_H
