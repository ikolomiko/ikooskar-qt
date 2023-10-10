#ifndef IKOOSKAR_BLL_SCHEMEEXPORTER_H
#define IKOOSKAR_BLL_SCHEMEEXPORTER_H
#include "Shared/deskcoordinates.h"
#include "Shared/scheme.h"

extern "C" {
#include "xlsxwriter.h"
}

namespace ikoOSKAR {
namespace BLL {
    using namespace Shared;
    class SchemeExporter : public QObject {
        Q_OBJECT

    private:
        Scheme scheme;

        void noDesk(lxw_workbook* workbook, lxw_worksheet* sheet, int xRow, int xCol);
        void emptyDesk(lxw_workbook* workbook, lxw_worksheet* sheet, DeskCoordinates& coord);
        void regularDesk(lxw_workbook* workbook, lxw_worksheet* sheet, DeskCoordinates& coord);
        void separator(lxw_workbook* workbook, lxw_worksheet* sheet, int xRow, int xCol, bool bothSidesEmpty);
        bool createDesk(lxw_workbook* workbook, lxw_worksheet* sheet, DeskCoordinates& coord);

    public:
        SchemeExporter(Scheme scheme);
        void exportClassLists();
        void exportHallLayouts();
        void exportAll();

    signals:
        void exportFinished(const QString& pathClassLists, const QString& pathHallLayouts);
    };

} // namespace BLL
} // namespace ikoOSKAR

#endif // IKOOSKAR_BLL_SCHEMEEXPORTER_H
