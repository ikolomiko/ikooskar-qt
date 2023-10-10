#include "monthheaderwidget.h"

namespace ikoOSKAR {
namespace UI {

    MonthHeaderWidget::MonthHeaderWidget(QDate date)
    {
        headerLine = new QFrame();
        label = new QLabel(QLocale().toString(date, "MMMM yyyy"));
        layout = new QHBoxLayout();

        label->setStyleSheet("font-size: 14pt;");

        headerLine->setFrameShape(QFrame::HLine);
        headerLine->setFrameShadow(QFrame::Sunken);
        headerLine->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Maximum);

        layout->addWidget(label);
        layout->addWidget(headerLine);

        setLayout(layout);
    }

} // namespace UI
} // namespace ikoOSKAR
