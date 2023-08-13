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

        ExamStudent(Student& s)
        {
            firstName = s.firstName;
            lastName = s.lastName;
            grade = s.grade;
            id = s.id;
            section = s.section;
        }

        bool operator<(const ExamStudent& other) const
        {
            return (id < other.id);
        }
    };

} // namespace Shared
} // namespace ikoOSKAR

#endif // IKOOSKAR_SHARED_EXAMSTUDENT_H
