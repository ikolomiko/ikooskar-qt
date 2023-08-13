#ifndef IKOOSKAR_UI_COMMON_TWOBUTTONNAV_H
#define IKOOSKAR_UI_COMMON_TWOBUTTONNAV_H

#include <QFrame>
#include <QPushButton>

namespace ikoOSKAR {
namespace UI {
    namespace Common {

        class TwoButtonNav : public QFrame {
            Q_OBJECT

        public:
            QPushButton* btnPrev;
            QPushButton* btnNext;

            explicit TwoButtonNav();
        };

    } // namespace Common
} // namespace UI
} // namespace ikoOSKAR

#endif // IKOOSKAR_UI_COMMON_TWOBUTTONNAV_H
