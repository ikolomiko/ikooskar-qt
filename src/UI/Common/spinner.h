#ifndef IKOOSKAR_UI_COMMON_SPINNER_H
#define IKOOSKAR_UI_COMMON_SPINNER_H

#include <QMovie>
#include <QWidget>

namespace ikoOSKAR {
namespace UI {
    namespace Common {

        namespace Ui {
            class Spinner;
        }

        class Spinner : public QWidget {
            Q_OBJECT

        public:
            explicit Spinner(QWidget* parent = nullptr);
            void setTitle(const QString& title);
            void start();
            void stop();
            ~Spinner();

        private:
            Ui::Spinner* ui;
            QMovie* spinnerGif;
        };

    } // namespace Common
} // namespace UI
} // namespace ikoOSKAR
#endif // IKOOSKAR_UI_COMMON_SPINNER_H
