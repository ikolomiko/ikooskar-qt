#include "pattern.h"
#include <QRandomGenerator>

namespace ikoOSKAR {
namespace Shared {
    Pattern::Pattern(int gradeCount, int seed)
        : gradeIndex(gradeCount - 1)
        , startOffset(seed)
    {
    }

    Pattern* Pattern::random(int gradeCount)
    {
        auto rng = QRandomGenerator::system();
        int rand = rng->bounded(0, 1 << 30);
        return new Pattern(gradeCount, rand);
    }

    Pattern::Variant Pattern::variantAt(int row, int col)
    {
        int actualRow = (row + startOffset) % 6;
        return patterns[gradeIndex][actualRow][col];
    }

} // namespace Shared
} // namespace ikoOSKAR
