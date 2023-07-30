#include "schemegenerator.h"
#include <QDir>
#include <QSettings>

namespace ikoOSKAR {
namespace BLL {

    SchemeGenerator::SchemeGenerator()
    {
    }

    QString SchemeGenerator::examDir()
    {
        return QSettings().value("PATH_DOCS_ROOT").toString() + "/Sınav Düzenleri/" + examDate.toString("dd.MM.yyyy") + "/" + examName;
    }

    bool SchemeGenerator::schemeExists()
    {
        return QDir(examDir()).exists();
    }

    void SchemeGenerator::setDate(const QDate& date)
    {
        examDate = date;
    }

    bool SchemeGenerator::setName(const QString& name)
    {
        if (name.isEmpty()) {
            emit error("Sınav adı boş bırakılamaz!");
            return false;
        }

        examName = name;
        if (schemeExists()) {
            emit error("Seçtiğiniz tarihte ve seçtiğiniz adda bir sınav düzeni zaten oluşturulmuş. Lütfen başka bir tarih ya da başka bir sınav adı seçiniz!");
            return false;
        }

        return true;
    }

} // namespace BLL
} // namespace ikoOSKAR
