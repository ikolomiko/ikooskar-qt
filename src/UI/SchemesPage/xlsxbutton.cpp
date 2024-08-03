#include "xlsxbutton.h"
#include "ui_xlsxbutton.h"
#include <QDesktopServices>
#include <QUrl>

namespace ikoOSKAR {
namespace UI {

    XlsxButton::XlsxButton(const QString& name, const QString& filePath, QWidget* parent)
        : QWidget(parent)
        , ui(new Ui::XlsxButton)
        , filePath(filePath)
    {
        ui->setupUi(this);
        ui->btnXlsxFile->setText(name);
    }

    XlsxButton::~XlsxButton()
    {
        delete ui;
    }

    void XlsxButton::on_btnXlsxFile_clicked()
    {
        QDesktopServices::openUrl(QUrl::fromLocalFile(filePath));
    }

} // namespace UI
} // namespace ikoOSKAR
