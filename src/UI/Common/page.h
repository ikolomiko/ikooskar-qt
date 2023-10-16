#ifndef IKOOSKAR_UI_COMMON_PAGE_H
#define IKOOSKAR_UI_COMMON_PAGE_H

#include <QLabel>
#include <QWidget>

namespace ikoOSKAR {
namespace UI {
    namespace Common {

        class Page : public QWidget {
            Q_OBJECT

        protected:
            QString* name;

        signals:
            void descriptionUpdated(const QString& description);

        public:
            virtual const QString* getDescription() = 0;

            const QString* getName()
            {
                return name;
            }

            explicit Page(QWidget* parent = nullptr)
                : QWidget(parent)
            {
            }
        };

    } // namespace Common
} // namespace UI
} // namespace ikoOSKAR

#endif // IKOOSKAR_UI_COMMON_PAGE_H
