#include "schemeexporter.h"

#include "xlsxcellrange.h"
#include "xlsxchart.h"
#include "xlsxchartsheet.h"
#include "xlsxdocument.h"
#include "xlsxrichstring.h"
#include "xlsxworkbook.h"

namespace ikoOSKAR {
namespace BLL {

    SchemeExporter::SchemeExporter(Scheme scheme)
        : scheme(scheme)
    {
    }

    void SchemeExporter::exportClassLists()
    {
    }

    void SchemeExporter::exportHallLayouts()
    {
    }

    void SchemeExporter::exportAll()
    {
        exportClassLists();
        exportHallLayouts();
    }

} // namespace BLL
} // namespace ikoOSKAR
