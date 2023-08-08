#ifndef IKOOSKAR_BLL_SCHEMEEXPORTER_H
#define IKOOSKAR_BLL_SCHEMEEXPORTER_H
#include "Shared/scheme.h"

namespace ikoOSKAR {
namespace BLL {
    using namespace Shared;
    class SchemeExporter {
    private:
        Scheme scheme;

    public:
        SchemeExporter(Scheme scheme);
        void exportClassLists();
        void exportHallLayouts();
        void exportAll();
    };

} // namespace BLL
} // namespace ikoOSKAR

#endif // IKOOSKAR_BLL_SCHEMEEXPORTER_H
