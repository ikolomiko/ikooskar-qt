#ifndef IKOOSKAR_SHARED_OGRENCI_H
#define IKOOSKAR_SHARED_OGRENCI_H
#include <QString>

namespace ikoOSKAR {
namespace Shared {

class Ogrenci
{
public:
    int Id;
    int OkulNo;
    QString Ad;
    QString Soyad;
    int Sinif;
    QString Sube;
};

} // namespace Shared
} // namespace ikoOSKAR

#endif // IKOOSKAR_SHARED_OGRENCI_H
