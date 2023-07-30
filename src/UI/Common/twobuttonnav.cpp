#include "twobuttonnav.h"
#include <QHBoxLayout>
#include <QSpacerItem>

namespace ikoOSKAR {
namespace UI {
    namespace Common {
        TwoButtonNav::TwoButtonNav()
            : btnPrev(new QPushButton())
            , btnNext(new QPushButton())
        {
            btnPrev->setObjectName("btnPrev");
            btnPrev->setText("  Geri  ");
            btnPrev->setIcon(QIcon(":/arrow-left.png"));
            btnPrev->setIconSize(QSize(22, 22));
            btnPrev->setMinimumSize(QSize(0, 40));

            auto hSpacer = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

            btnNext->setObjectName("btnNext");
            btnNext->setText("  İleri  ");
            btnNext->setIcon(QIcon(":/arrow-right.png"));
            btnNext->setIconSize(QSize(22, 22));
            btnNext->setMinimumSize(QSize(0, 40));
            btnNext->setLayoutDirection(Qt::RightToLeft);

            auto layout = new QHBoxLayout();
            layout->addWidget(btnPrev);
            layout->addSpacerItem(hSpacer);
            layout->addWidget(btnNext);
            layout->setContentsMargins(0, 0, 0, 0);

            this->setMaximumHeight(40);
            this->setFrameShape(NoFrame);
            this->setLayout(layout);
        }

    } // namespace Common
} // namespace UI
} // namespace ikoOSKAR
