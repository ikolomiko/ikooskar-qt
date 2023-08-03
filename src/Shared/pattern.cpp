#include "pattern.h"
#include <QRandomGenerator>

namespace ikoOSKAR {
namespace Shared {
    Pattern::Pattern(QList<int> grades, int seed)
        : gradeIndex(grades.count() - 1)
        , startOffset(seed)
    {
        // Generate variant->grade mappings

        // Don't randomly map if the seed is 0
        if (seed == 0) {
            for (int i = 0; i < grades.count(); i++) {
                Variant v = (Variant)i;
                int grade = grades.at(i);
                variantGradeMappings.insert(v, grade);
            }
        } else {
            // Randomly select and remove an item from the list
            const auto randomSelection = [](QList<int>* list) {
                int rand = QRandomGenerator::system()->generate();
                int idx = rand % list->count();
                int result = list->at(idx);
                list->removeAt(idx);

                return result;
            };

            int originalSize = grades.size();
            for (int i = 0; i < originalSize; i++) {
                Variant v = (Variant)i;
                int grade = randomSelection(&grades);
                variantGradeMappings.insert(v, grade);
            }
        }
    }

    Pattern* Pattern::random(QList<int> grades)
    {
        int rand = QRandomGenerator::system()->generate();
        return new Pattern(grades, rand);
    }

    int Pattern::mapToGrade(Variant v)
    {
        return variantGradeMappings.value(v);
    }

    Pattern::Variant Pattern::variantAt(int row, int col)
    {
        int actualRow = (row + startOffset) % 6;
        return patterns[gradeIndex][actualRow][col];
    }

    int Pattern::gradeAt(int row, int col)
    {
        return mapToGrade(variantAt(row, col));
    }

} // namespace Shared
} // namespace ikoOSKAR
