#include "multiimporthelper.h"
#include "BLL/DatabaseHelper/databasehelper.h"
#include <QMap>

namespace ikoOSKAR {
namespace BLL {

    MultiImportHelper::MultiImportHelper(const QString& xlsFilePath)
        : dal(new ikoOSKAR::DAL::MultiImport)
        , xlsFilePath(xlsFilePath)
    {
    }

    void MultiImportHelper::run()
    {
        auto result = parseXls();
        emit parsingFinished(result);
    }

    bool MultiImportHelper::matchesHeaderRow(const QStringList& line)
    {
        const QStringList headerRow = { "S.No", "Öğrenci No", "Adı", "Soyadı", "Cinsiyeti" };
        if (line.size() < headerRow.size()) {
            return false;
        }

        for (int i = 0; i < headerRow.size(); i++) {
            if (line[i] != headerRow[i]) {
                return false;
            }
        }

        return true;
    }

    bool MultiImportHelper::matchesSectionFooter(const QStringList& line)
    {
        return line.at(0).contains("Öğrenci Sayısı");
    }

    bool MultiImportHelper::matchesFileFooter(const QList<QStringList>& lines)
    {
        // There must be at least 3 rows (1 header row, 1 section footer row and 1 (or 2) file footer row(s))
        // and the last row must have 3 non-empty cells
        const int& nLines = lines.size();
        return nLines >= 3 and (lines.at(nLines - 1).size() == 3 or lines.at(nLines - 2).size() == 3);
    }

    QList<Student*>* MultiImportHelper::parseXls()
    {
        bool isFileReadable = dal->importXlsFile(xlsFilePath);
        if (!isFileReadable) {
            emit error("Hata: Excel dosyası okunamadı! Dosya yok ya da okuma izni yok.");
            return nullptr;
        }

        const auto& lines = dal->getTrimmedLines();
        if (lines == nullptr) {
            emit error("Hata: Excel dosyası okunamadı! Dosya arızalı.");
            return nullptr;
        }

        const int& nLines = lines->size();
        if (nLines < 2) {
            // The first row is treated as headers row,
            // so we will ignore it
            emit error("Hata: Excel dosyası boş!");
            return nullptr;
        }

        int startingLine = 0;
        for (int i = 0; i < lines->count(); i++) {
            if (matchesHeaderRow(lines->at(i))) {
                startingLine = i;
                break;
            }
        }

        int endingLine = nLines - 1;
        for (int i = nLines - 1; i > 0; i--) {
            if (matchesSectionFooter(lines->at(i))) {
                endingLine = i;
                break;
            }
        }

        auto students = QMap<int, Student*>();

        bool isStandardFormat = matchesHeaderRow(lines->at(startingLine))
            && matchesSectionFooter(lines->at(endingLine))
            && matchesFileFooter(*lines);

        if (isStandardFormat) {
            startingLine++;
        }

        for (int i = startingLine; i <= endingLine; i++) {
            const auto& line = lines->at(i);

            if (line.size() == 0) {
                // Ignore empty rows
                continue;
            }

            if (isStandardFormat) {
                if (matchesSectionFooter(line)) {
                    if (i == endingLine) {
                        // Hit the end of the file
                        break;
                    } else {
                        // Found another section footer, meaning there are multiple sections
                        emit error("Hata: Excel dosyası birden fazla sınıf/şube içeriyor. Tek seferde birden fazla sınıf/şube eklenemez!");
                        return nullptr;
                    }
                }
            }

            QString errorMsg = QString("Excel dosyasındaki %1. satırda hata oluştu: ").arg(i + 1);

            if (line.size() < 4) {
                // When not processing any header or footer row, there must be at least 4 cells each row
                errorMsg += "Öğrenci bilgisi içeren her satırda en az 4 sütun bulunmalıdır! (Sınıf sıra no, okul no, ad, soyad, ...)";
                emit error(errorMsg);
                return nullptr;
            }

            // The first cell of each row is discarded
            bool success = false;
            Student* s = new Student();
            s->id = line.at(1).toInt(&success);
            s->firstName = line.at(2);
            s->lastName = line.at(3);

            // Check for student id being a number
            if (!success) {
                errorMsg += "Öğrenci numarası bir sayı olmalıdır!";
                emit error(errorMsg);
                return nullptr;
            }

            // Check for first name being empty
            if (s->firstName.isEmpty()) {
                errorMsg += "Öğrenci adı boş olamaz!";
                emit error(errorMsg);
                return nullptr;
            }

            // Check for last name being empty
            if (s->lastName.isEmpty()) {
                errorMsg += "Öğrenci soyadı boş olamaz!";
                emit error(errorMsg);
                return nullptr;
            }

            // Check for duplicate student IDs in the file itself
            if (students.contains(s->id)) {
                errorMsg += QString("Bu Excel dosyasında aynı öğrenci no'ya (%1) sahip birden fazla öğrenci var!").arg(s->id);
                emit error(errorMsg);
                return nullptr;
            }

            // Check for duplicate student IDs between the file and the database
            const auto db = BLL::DatabaseHelper::getInstance();
            if (db->IdExists(s->id)) {
                errorMsg += QString("Sisteme kayıtlı öğrenciler arasında aynı okul no'ya (%1) sahip başka bir öğrenci daha var!").arg(s->id);
                emit error(errorMsg);
                return nullptr;
            }

            students[s->id] = s;
        }

        return new QList(students.values());
    }

    MultiImportHelper::~MultiImportHelper()
    {
        delete this->dal;
    }

} // namespace BLL
} // namespace ikoOSKAR
