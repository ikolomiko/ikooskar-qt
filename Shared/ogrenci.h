#ifndef IKOOSKAR_SHARED_OGRENCI_H
#define IKOOSKAR_SHARED_OGRENCI_H
#include <QString>

namespace ikoOSKAR {
namespace Shared {

class Ogrenci
{
public:
    int OkulNo;
    QString Ad;
    QString Soyad;
    int Sinif;
    QChar Sube;
};

} // namespace Shared
} // namespace ikoOSKAR

#endif // IKOOSKAR_SHARED_OGRENCI_H
