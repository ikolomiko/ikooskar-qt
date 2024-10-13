#ifndef IKOOSKAR_SHARED_DESK_H
#define IKOOSKAR_SHARED_DESK_H
#include "examstudent.h"
#include <optional>

namespace ikoOSKAR {
namespace Shared {

    class Desk {
    public:
        std::optional<ExamStudent> student;
        bool exists;
        bool isEmpty;

        Desk()
            : student({})
            , exists(true)
            , isEmpty(true)
        {
        }

        Desk(bool exists)
            : student({})
            , exists(exists)
            , isEmpty(true)
        {
        }
    };

} // namespace Shared
} // namespace ikoOSKAR

#endif // IKOOSKAR_SHARED_DESK_H
