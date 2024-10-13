#ifndef IKOOSKAR_SHARED_DESKCOORDINATES_H
#define IKOOSKAR_SHARED_DESKCOORDINATES_H
#include "hall.h"

namespace ikoOSKAR {
namespace Shared {

    class DeskCoordinates {
    public:
        Hall* hall;
        size_t row;
        size_t col;

        DeskCoordinates(Hall* hall, size_t row, size_t col)
            : hall(hall)
            , row(row)
            , col(col)
        {
        }

        DeskCoordinates(Hall* hall, int deskIndex)
            : hall(hall)
            , row((deskIndex - 1) / 6)
            , col((deskIndex - 1) % 6)
        {
        }

        bool operator==(const DeskCoordinates& other) const
        {
            return hall == other.hall && row == other.row && col == other.col;
        }
    };

} // namespace Shared
} // namespace ikoOSKAR

#endif // IKOOSKAR_SHARED_DESKCOORDINATES_H
