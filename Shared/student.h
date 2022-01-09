#ifndef IKOOSKAR_SHARED_STUDENT_H
#define IKOOSKAR_SHARED_STUDENT_H
#include <QString>

namespace ikoOSKAR {
namespace Shared {

    class Student {
    public:
        int id;
        QString firstName;
        QString lastName;
        int grade;
        QString section;
    };

} // namespace Shared
} // namespace ikoOSKAR

#endif // IKOOSKAR_SHARED_STUDENT_H
