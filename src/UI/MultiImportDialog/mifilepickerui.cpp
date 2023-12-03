#include "mifilepickerui.h"
#include "ui_mifilepickerui.h"
#include <QDesktopServices>
#include <QFileDialog>
#include <QStandardPaths>
#include <QUrl>

namespace ikoOSKAR {
namespace UI {

    MIFilePickerUi::MIFilePickerUi(QWidget* parent)
        : QWidget(parent)
        , ui(new Ui::MIFilePickerUi)
    {
        ui->setupUi(this);
    }

    MIFilePickerUi::~MIFilePickerUi()
    {
        delete ui;
    }

    void MIFilePickerUi::on_btnOpenFile_clicked()
    {
        QFileDialog dialog(this);
        dialog.setFileMode(QFileDialog::ExistingFile);
        dialog.setNameFilters({ "E-Okul Excel - Sadece Veri (*.XLS *.xls)", "Elle Oluşturulmuş Excel Dosyası (*.XLSX *.xlsx)" });
        dialog.setViewMode(QFileDialog::Detail);
        dialog.setDirectory(QStandardPaths::writableLocation(QStandardPaths::DownloadLocation));

        if (dialog.exec() == QDialog::Accepted) {
            emit pickedXlsFile(new QString(dialog.selectedFiles().at(0)));
        }
    }

    void MIFilePickerUi::on_btnHelpExcel_clicked()
    {
        QDesktopServices::openUrl(QUrl("https://youtu.be/D8Ao18qsnJ0"));
    }

} // namespace UI
} // namespace ikoOSKAR
