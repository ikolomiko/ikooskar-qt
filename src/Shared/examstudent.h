#ifndef IKOOSKAR_SHARED_EXAMSTUDENT_H
#define IKOOSKAR_SHARED_EXAMSTUDENT_H
#include "student.h"
#include <QString>

namespace ikoOSKAR {
namespace Shared {

    class ExamStudent : public Student {
    public:
        QString hallName;
        int deskIndex;
    };

} // namespace Shared
} // namespace ikoOSKAR

#endif // IKOOSKAR_SHARED_EXAMSTUDENT_H
