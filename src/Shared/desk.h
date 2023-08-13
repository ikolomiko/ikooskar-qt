#ifndef IKOOSKAR_SHARED_DESK_H
#define IKOOSKAR_SHARED_DESK_H
#include "examstudent.h"

namespace ikoOSKAR {
namespace Shared {

    class Desk {
    public:
        ExamStudent* student;
        bool exists;
        bool isEmpty;

        Desk()
            : student(nullptr)
            , exists(true)
            , isEmpty(true)
        {
        }

        Desk(bool exists)
            : student(nullptr)
            , exists(exists)
            , isEmpty(true)
        {
        }
    };

} // namespace Shared
} // namespace ikoOSKAR

#endif // IKOOSKAR_SHARED_DESK_H
