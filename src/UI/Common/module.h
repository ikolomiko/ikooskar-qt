#ifndef IKOOSKAR_UI_COMMON_MODULE_H
#define IKOOSKAR_UI_COMMON_MODULE_H

#include <QLabel>
#include <QWidget>

namespace ikoOSKAR {
namespace UI {
    namespace Common {

        class Module : public QWidget {
            Q_OBJECT

        protected:
            QString* name;
            QLabel* lblDescription;

        public:
            void setDescriptionLabel(QLabel* lbl);
            const QString* getName();
            virtual const QString* getDescription() = 0;
            explicit Module(QWidget* parent = nullptr);
        };

    } // namespace Common
} // namespace UI
} // namespace ikoOSKAR

#endif // IKOOSKAR_UI_COMMON_MODULE_H
