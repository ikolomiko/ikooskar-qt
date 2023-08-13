#ifndef IKOOSKAR_SHARED_PATTERN_H
#define IKOOSKAR_SHARED_PATTERN_H

#include <QHash>
#include <QList>

namespace ikoOSKAR {
namespace Shared {

    class Pattern {
    public:
        enum Variant {
            a = 0,
            b,
            c,
            d
        };

        Pattern(int gradeCount, int seed);
        static Pattern* random(int gradeCount);
        Variant variantAt(int row, int col);

    private:
        int gradeIndex; // gradeIndex = gradeCount - 1
        int startOffset;

        // clang-format off
        const static inline Variant patterns[4][6][6] = {
            {   // 1-variant pattern
                { a, a, a, a, a, a },
                { a, a, a, a, a, a },
                { a, a, a, a, a, a },
                { a, a, a, a, a, a },
                { a, a, a, a, a, a },
                { a, a, a, a, a, a }
            },
            {   // 2-variant pattern
                { a, b, a, b, a, b },
                { a, b, a, b, a, b },
                { a, b, a, b, a, b },
                { a, b, a, b, a, b },
                { a, b, a, b, a, b },
                { a, b, a, b, a, b }
            },
            {   // 3-variant pattern
                { a, b, a, b, a, b },
                { a, c, a, c, a, c },
                { b, c, b, c, b, c },
                { b, a, b, a, b, a },
                { c, a, c, a, c, a },
                { c, b, c, b, c, b }
            },
            {   // 4-variant pattern
                { a, b, c, d, a, b },
                { c, d, a, b, c, d },
                { a, b, c, d, a, b },
                { c, d, a, b, c, d },
                { a, b, c, d, a, b },
                { c, d, a, b, c, d }
            }
        };
        // clang-format on
    };

} // namespace Shared
} // namespace ikoOSKAR

#endif // IKOOSKAR_SHARED_PATTERN_H
