#include "contactwidget.h"
#include "ui_contactwidget.h"

namespace ikoOSKAR {
namespace UI {
    namespace Common {

        ContactWidget::ContactWidget(const QPixmap& image, const QString& text, const QString& href, QWidget* parent)
            : QWidget(parent)
            , ui(new Ui::ContactWidget)
        {
            ui->setupUi(this);

            ui->image->setPixmap(image.scaled(32, 32, Qt::KeepAspectRatioByExpanding, Qt::SmoothTransformation));
            ui->label->setText("<a href=\"" + href + "\">" + text + "</a>");
            ui->label->setTextFormat(Qt::RichText);
            ui->label->setOpenExternalLinks(true);
        }

        ContactWidget::~ContactWidget()
        {
            delete ui;
        }

    } // namespace Common
} // namespace UI
} // namespace ikoOSKAR
