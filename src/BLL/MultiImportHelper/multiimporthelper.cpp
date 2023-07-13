#include "multiimporthelper.h"
#include "BLL/DatabaseHelper/databasehelper.h"
#include <QHash>
#include <QProcess>

namespace ikoOSKAR {
namespace BLL {

    MultiImportHelper::MultiImportHelper(UI::ErrorUi* errorUi)
        : errorUi(errorUi)
    {
        dal = new ikoOSKAR::DAL::MultiImport();
    }

    QList<Student*>* MultiImportHelper::parseXls(const QString& xlsFilePath)
    {
        QString* csvFilePath = convertToCsv(xlsFilePath);
        if (csvFilePath == nullptr || !QFile::exists(*csvFilePath)) {
            errorUi->DisplayMessage("Hata: Excel dosyası okunamadı!");
            return nullptr;
        }

        return parseCsv();
    }

    QString* MultiImportHelper::convertToCsv(const QString& xlsFilePath)
    {
        dal->importXlsFile(xlsFilePath);
        DAL::OfficeSuite officeSuite = DAL::LibreOffice;

        QString* csvConverterPath = dal->getCsvConverterPath(officeSuite);
        if (csvConverterPath == nullptr) {
            officeSuite = DAL::MsOffice;
            csvConverterPath = dal->getCsvConverterPath(officeSuite);
            if (csvConverterPath == nullptr) {
                errorUi->DisplayMessage("Hata: Yüklü Office programı bulunamadı! Lütfen LibreOffice ya da Microsoft Office yükleyin.");
                return nullptr;
            }
        }

        QStringList* csvConverterArgs = dal->getCsvConverterArgs();

        QProcess convert;
        convert.start(*csvConverterPath, *csvConverterArgs);
        convert.waitForFinished();

        return dal->getCsvPath();
    }

    bool MultiImportHelper::matchesHeaderRow(const QStringList& line)
    {
        const QStringList headerRow = { "S.No", "Öğrenci No", "Adı", "Soyadı", "Cinsiyeti" };
        if (line.size() != headerRow.size()) {
            return false;
        }

        for (int i = 0; i < line.size(); i++) {
            if (line[i] != headerRow[i]) {
                return false;
            }
        }

        return true;
    }

    bool MultiImportHelper::matchesSectionFooter(const QStringList& line)
    {
        if (line.size() != 6) {
            return false;
        }
        return line.at(0).contains("Öğrenci Sayısı");
    }

    bool MultiImportHelper::matchesFileFooter(const QList<QStringList>& lines)
    {
        // There must be at least 3 rows (1 header row, 1 section footer row and 1 file footer row)
        // and the last row must have 3 non-empty cells
        const int& nLines = lines.size();
        return nLines >= 3 && lines.at(nLines - 1).size() == 3;
    }

    QList<Student*>* MultiImportHelper::parseCsv()
    {
        const auto& lines = dal->getTrimmedCsvLines();
        const int& nLines = lines->size();
        if (nLines < 2) {
            // The first row is treated as headers row,
            // so we will ignore it
            errorUi->DisplayMessage("Hata: Excel dosyası boş!");
            return nullptr;
        }

        auto students = QHash<int, Student*>();

        bool isStandardFormat = matchesHeaderRow(lines->at(0))
            && matchesSectionFooter(lines->at(nLines - 2))
            && matchesFileFooter(*lines);

        for (int i = 1; i < nLines; i++) {
            const auto& line = lines->at(i);

            if (line.size() == 0) {
                // Ignore empty rows
                continue;
            }

            if (isStandardFormat) {
                if (matchesSectionFooter(line)) {
                    if (i == nLines - 2) {
                        // Hit the end of the file
                        break;
                    } else {
                        // Found another section footer, meaning there are multiple sections
                        errorUi->DisplayMessage("Hata: Tek seferde birden fazla sınıf/şube eklenemez!");
                        return nullptr;
                    }
                }
            }

            if (line.size() < 4) {
                // When not processing any header or footer row, there must be at least 4 cells each row
                errorUi->DisplayMessage(QString::number(i + 1)
                    + ". satırda hata oluştu: Öğrenci bilgisi içeren her satırda en az 4 sütun bulunmalıdır! (Sınıf sıra no, okul no, ad, soyad, ...)");
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
                errorUi->DisplayMessage(QString::number(i + 1)
                    + ". satırda hata oluştu: Öğrenci numarası bir sayı olmalıdır!");
                return nullptr;
            }

            // Check for first name being empty
            if (s->firstName.isEmpty()) {
                errorUi->DisplayMessage(QString::number(i + 1)
                    + ". satırda hata oluştu: Öğrenci adı boş olamaz!");
                return nullptr;
            }

            // Check for last name being empty
            if (s->lastName.isEmpty()) {
                errorUi->DisplayMessage(QString::number(i + 1)
                    + ". satırda hata oluştu: Öğrenci soyadı boş olamaz!");
                return nullptr;
            }

            // Check for duplicate student IDs in the file itself
            if (students.contains(s->id)) {
                errorUi->DisplayMessage(QString::number(i + 1)
                    + ". satırda hata oluştu: Bu Excel dosyasında "
                    + QString::number(s->id)
                    + " öğrenci no'suna sahip başka bir öğrenci daha var!");
                return nullptr;
            }

            // Check for duplicate student IDs between the file and the database
            const auto db = BLL::DatabaseHelper::getInstance(errorUi);
            if (db->IdExists(s->id)) {
                errorUi->DisplayMessage(QString::number(i + 1)
                    + ". satırda hata oluştu: Sistemde "
                    + QString::number(s->id)
                    + " öğrenci numarasına sahip başka bir öğrenci kayıtlı!");
                return nullptr;
            }

            students[s->id] = s;
        }

        return new QList(students.values());
    }

    MultiImportHelper::~MultiImportHelper()
    {
        delete this->dal;
        delete this->errorUi;
    }

} // namespace BLL
} // namespace ikoOSKAR
