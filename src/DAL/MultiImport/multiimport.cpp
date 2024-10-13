#include "multiimport.h"
#include <QDebug>
#include <QFile>
#include <QFileInfo>

extern "C" {
#include "deps/freexl/freexl.h"
}

namespace ikoOSKAR {
namespace DAL {

    MultiImport::MultiImport()
    {
    }

    QStringList MultiImport::trimStringList(const QStringList& list)
    {
        QStringList result;

        for (const auto& token : list) {
            const auto simplified = token.simplified();
            if (!simplified.isEmpty()) {
                result.append(simplified);
            }
        }

        return result;
    }

    bool MultiImport::importXlsFile(const QString& xlsFilePath)
    {
        this->xlsFilePath = xlsFilePath;
        QFile file(xlsFilePath);

        // return if the file is readable
        return file.exists() && file.open(QIODevice::ReadOnly);
    }

    QList<QStringList>* MultiImport::getTrimmedLines()
    {
        const void* workbook;
        int ret;

        auto bPath = xlsFilePath.toUtf8();
        QString extension = QFileInfo(xlsFilePath).suffix().toLower();

        if (extension == "xls") {
            ret = freexl_open(bPath.data(), &workbook);
        } else { // extension == "xlsx"
            ret = freexl_open_xlsx(bPath.data(), &workbook);
        }

        if (ret != FREEXL_OK) {
            qDebug() << "could not open " << extension << " file " << ret;
            // Could not open the XLS(X) file
            freexl_close(workbook);
            return nullptr;
        }
        freexl_select_active_worksheet(workbook, 0);

        uint16_t cols;
        uint32_t rows;
        ret = freexl_worksheet_dimensions(workbook, &rows, &cols);
        if (ret != FREEXL_OK) {
            qDebug() << "could not read dimensions " << ret;
            // Could not read dimensions
            freexl_close(workbook);
            return nullptr;
        }

        const auto lines = new QList<QStringList>();
        for (uint32_t row = 0; row < rows; row++) {
            QStringList line;
            FreeXL_CellValue cell;
            for (uint16_t col = 0; col < cols; col++) {
                QString cellValue;
                ret = freexl_get_cell_value(workbook, row, col, &cell);
                if (ret != FREEXL_OK) {
                    // Some error happened while reading cell[row,col]
                    cellValue = "HATA " + QString::number(ret);
                } else {
                    switch (cell.type) {
                    case FREEXL_CELL_DATE:
                    case FREEXL_CELL_DATETIME:
                    case FREEXL_CELL_TIME:
                    case FREEXL_CELL_TEXT:
                    case FREEXL_CELL_SST_TEXT:
                        cellValue = cell.value.text_value;
                        break;
                    case FREEXL_CELL_INT:
                        cellValue = QString::number(cell.value.int_value);
                        break;
                    case FREEXL_CELL_DOUBLE:
                        cellValue = QString::number((uint64_t)cell.value.double_value);
                        break;
                    default:
                        cellValue = "";
                    }
                }

                line.append(cellValue);
            }
            const auto& trimmed = trimStringList(line);
            if (trimmed.size() > 0) {
                lines->append(trimmed);
            }
        }

        freexl_close(workbook);
        return lines;
    }

} // namespace DAL
} // namespace ikoOSKAR
