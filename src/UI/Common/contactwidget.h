#ifndef IKOOSKAR_UI_COMMON_CONTACTWIDGET_H
#define IKOOSKAR_UI_COMMON_CONTACTWIDGET_H

#include <QWidget>

namespace ikoOSKAR {
namespace UI {
    namespace Common {

        namespace Ui {
            class ContactWidget;
        }

        class ContactWidget : public QWidget {
            Q_OBJECT

        public:
            explicit ContactWidget(const QPixmap& image, const QString& text, const QString& href, QWidget* parent = nullptr);
            ~ContactWidget();

        private:
            Ui::ContactWidget* ui;
        };

    } // namespace Common
} // namespace UI
} // namespace ikoOSKAR
#endif // IKOOSKAR_UI_COMMON_CONTACTWIDGET_H
