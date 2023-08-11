#include "schemeexporter.h"
#include <QColor>
#include <QDir>

#define GRAY (0xaaaaaa)
#define LIGHT_GRAY (0xd3d3d3)
#define WHITE_SMOKE (0xf5f5f5)
#define PAPER_A4 (9)

namespace ikoOSKAR {
namespace BLL {

    void SchemeExporter::noDesk(lxw_workbook* workbook, lxw_worksheet* sheet, int xRow, int xCol)
    {
        lxw_format* format = workbook_add_format(workbook);
        format_set_border(format, LXW_BORDER_DOTTED);

        worksheet_write_string(sheet, xRow, xCol, "", format);
    }

    void SchemeExporter::emptyDesk(lxw_workbook* workbook, lxw_worksheet* sheet, DeskCoordinates& coord)
    {
        lxw_format* format = workbook_add_format(workbook);
        format_set_border(format, LXW_BORDER_THIN);
        format_set_bg_color(format, LIGHT_GRAY);
        format_set_bold(format);
        format_set_font_size(format, 11);
        format_set_align(format, LXW_ALIGN_CENTER);
        format_set_align(format, LXW_ALIGN_VERTICAL_TOP);

        int xRow = 3 + 2 * coord.row;
        int xCol = 2 * coord.col;
        int deskIdx = (coord.row * 6) + coord.col + 1;
        char* txt = (QString::number(deskIdx) + "\n\nBOŞ").toUtf8().data();

        worksheet_write_string(sheet, xRow, xCol, txt, format);
    }

    void SchemeExporter::regularDesk(lxw_workbook* workbook, lxw_worksheet* sheet, DeskCoordinates& coord)
    {
        lxw_format* format = workbook_add_format(workbook);
        format_set_border(format, LXW_BORDER_THIN);
        format_set_bg_color(format, WHITE_SMOKE);
        format_set_align(format, LXW_ALIGN_VERTICAL_TOP);
        format_set_align(format, LXW_ALIGN_CENTER);
        format_set_text_wrap(format);

        lxw_format* bold = workbook_add_format(workbook);
        format_set_bold(bold);
        format_set_font_size(bold, 14);
        lxw_format* regular = nullptr;

        auto student = coord.hall->layout.desks[coord.row][coord.col]->student;
        int xRow = 3 + 2 * coord.row;
        int xCol = 2 * coord.col;
        int deskIndex = student->deskIndex;

        QByteArray bUpperText = QString::number(deskIndex).toUtf8();
        char* upperText = bUpperText.data();
        // clang-format off
        char* lowerText = QString("\n%1 - %2/%3\n%4 %5").arg(
            QString::number(student->id),
            QString::number(student->grade),
            student->section,
            student->firstName,
            student->lastName
        ).toUtf8().data();
        // clang-format on

        lxw_rich_string_tuple upper = { .format = bold, .string = upperText };
        lxw_rich_string_tuple lower = { .format = regular, .string = lowerText };
        lxw_rich_string_tuple* txt[] = {
            &upper,
            &lower,
            nullptr
        };
        worksheet_write_rich_string(sheet, xRow, xCol, txt, format);
    }

    void SchemeExporter::separator(lxw_workbook* workbook, lxw_worksheet* sheet, int xRow, int xCol, bool bothSidesEmpty)
    {
        lxw_format* format = workbook_add_format(workbook);
        format_set_right(format, LXW_BORDER_DOTTED);
        format_set_left(format, LXW_BORDER_DOTTED);
        if (bothSidesEmpty) {
            format_set_top(format, LXW_BORDER_DOTTED);
            format_set_bottom(format, LXW_BORDER_DOTTED);
            format_set_bg_color(format, LXW_COLOR_WHITE);
        } else {
            format_set_top(format, LXW_BORDER_THIN);
            format_set_bottom(format, LXW_BORDER_THIN);
            format_set_bg_color(format, LIGHT_GRAY);
        }

        worksheet_write_string(sheet, xRow, xCol, "", format);
    }

    bool SchemeExporter::createDesk(lxw_workbook* workbook, lxw_worksheet* sheet, DeskCoordinates& coord)
    {
        auto& desk = coord.hall->layout.desks[coord.row][coord.col];
        if (!desk->exists) {
            int xRow = 3 + 2 * coord.row;
            int xCol = 2 * coord.col;
            noDesk(workbook, sheet, xRow, xCol);
            return false;
        }

        if (desk->isEmpty) {
            emptyDesk(workbook, sheet, coord);
        } else {
            regularDesk(workbook, sheet, coord);
        }

        return true;
    }

    SchemeExporter::SchemeExporter(Scheme scheme)
        : scheme(scheme)
    {
        QDir().mkpath(scheme.path());
        pathClassLists = QDir::toNativeSeparators(scheme.path() + "/Sınıf Karma Listeleri.xlsx");
        pathHallLayouts = QDir::toNativeSeparators(scheme.path() + "/Oturma Planları.xlsx");
    }

    void SchemeExporter::exportClassLists()
    {
        auto bPath = pathClassLists.toLocal8Bit();
        char* path = bPath.data();
        lxw_workbook* workbook = workbook_new(path);

        for (const auto& [className, studentList] : scheme.classLists) {
            lxw_worksheet* sheet = workbook_add_worksheet(workbook, className.toUtf8());

            // Merge cells
            worksheet_merge_range(sheet, 0, 0, 0, 4, "", nullptr);
            worksheet_merge_range(sheet, 1, 0, 1, 1, "", nullptr);
            worksheet_merge_range(sheet, 1, 2, 1, 4, "", nullptr);

            // Set column widths
            worksheet_set_column(sheet, 0, 0, 12, nullptr);
            worksheet_set_column(sheet, 1, 2, 30, nullptr);
            worksheet_set_column(sheet, 3, 3, 13, nullptr);

            // Set row heights
            worksheet_set_row(sheet, 0, 40, nullptr);
            worksheet_set_row(sheet, 1, 21, nullptr);
            worksheet_set_row(sheet, 2, 21, nullptr);

            {
                // Header cell
                lxw_format* format = workbook_add_format(workbook);
                format_set_bold(format);
                format_set_font_size(format, 32);
                format_set_align(format, LXW_ALIGN_CENTER);
                format_set_border(format, LXW_BORDER_THIN);
                for (int col = 1; col < 5; col++) {
                    worksheet_write_string(sheet, 0, col, "", format);
                }
                worksheet_write_string(sheet, 0, 0, (className + " SINIFI KARMA LİSTESİ").toUtf8(), format);
            }

            {
                // Row 2: exam name and date
                lxw_format* format = workbook_add_format(workbook);
                format_set_bold(format);
                format_set_border(format, LXW_BORDER_THIN);
                format_set_font_size(format, 15);
                format_set_bg_color(format, GRAY);
                worksheet_write_string(sheet, 1, 0, ("Sınavın Adı: " + scheme.name).toUtf8(), format);

                lxw_format* format2 = workbook_add_format(workbook);
                format_set_bold(format2);
                format_set_border(format2, LXW_BORDER_THIN);
                format_set_left(format2, LXW_BORDER_NONE);
                format_set_font_size(format2, 15);
                format_set_bg_color(format2, GRAY);
                format_set_align(format2, LXW_ALIGN_RIGHT);
                for (int col = 2; col < 5; col++) {
                    worksheet_write_string(sheet, 1, col, "", format2);
                }
                worksheet_write_string(sheet, 1, 2, ("Sınav Tarihi: " + scheme.date).toUtf8(), format2);
            }

            {
                // Row 3: data headers
                lxw_format* format = workbook_add_format(workbook);
                format_set_bold(format);
                format_set_font_size(format, 15);
                format_set_border(format, LXW_BORDER_THIN);
                format_set_align(format, LXW_ALIGN_LEFT);
                format_set_bg_color(format, LIGHT_GRAY);

                worksheet_write_string(sheet, 2, 0, "Okul No", format);
                worksheet_write_string(sheet, 2, 1, "Ad", format);
                worksheet_write_string(sheet, 2, 2, "Soyad", format);
                worksheet_write_string(sheet, 2, 3, "Derslik", format);
                worksheet_write_string(sheet, 2, 4, "Sıra", format);
            }

            {
                // Row 4 and onwards
                int row = 3;
                for (const auto& s : studentList) {
                    lxw_format* format = workbook_add_format(workbook);
                    format_set_border(format, LXW_BORDER_THIN);
                    format_set_align(format, LXW_ALIGN_LEFT);
                    format_set_font_size(format, 12);

                    worksheet_write_number(sheet, row, 0, s.id, format);
                    worksheet_write_string(sheet, row, 1, s.firstName.toUtf8(), format);
                    worksheet_write_string(sheet, row, 2, s.lastName.toUtf8(), format);
                    worksheet_write_string(sheet, row, 3, s.hallName.toUtf8(), format);
                    worksheet_write_number(sheet, row, 4, s.deskIndex, format);

                    row++;
                }
            }

            worksheet_fit_to_pages(sheet, 1, 1);
            worksheet_center_horizontally(sheet);
            worksheet_set_portrait(sheet);
            worksheet_set_paper(sheet, PAPER_A4);
            worksheet_set_margins(sheet, 0.24, 0.24, 0.24, 0.24); // 0.24 inch ~= 0.6 cm
        }

        workbook_close(workbook);
    }

    void SchemeExporter::exportHallLayouts()
    {
        auto bPath = pathHallLayouts.toLocal8Bit();
        char* path = bPath.data();
        lxw_workbook* workbook = workbook_new(path);

        for (auto& hall : scheme.hallLayouts) {
            lxw_worksheet* sheet = workbook_add_worksheet(workbook, hall.name.toUtf8());

            // Set column widths
            for (int col = 0; col < 11; col++) {
                int width = (col % 2 == 1) ? 1 : 16;
                worksheet_set_column(sheet, col, col, width, nullptr);
            }

            // Set row heights
            worksheet_set_row(sheet, 0, 40, nullptr);
            worksheet_set_row(sheet, 1, 21, nullptr);
            worksheet_set_row(sheet, 2, 14, nullptr);

            {
                // Row 1: main header
                char* txt = (hall.name + " DERSLİK OTURMA PLANI").toUtf8().data();
                lxw_format* format = workbook_add_format(workbook);
                format_set_bold(format);
                format_set_font_size(format, 32);
                format_set_align(format, LXW_ALIGN_CENTER);
                worksheet_merge_range(sheet, 0, 0, 0, 10, txt, format);
            }

            {
                // Row 2: sub header including exam name and date
                char* txt = ("Sınavın Adı: " + scheme.name).toUtf8().data();
                lxw_format* format = workbook_add_format(workbook);
                format_set_bold(format);
                format_set_font_size(format, 16);
                format_set_bg_color(format, GRAY);
                worksheet_merge_range(sheet, 1, 0, 1, 5, txt, format);

                char* txt2 = ("Sınav Tarihi: " + scheme.date).toUtf8().data();
                lxw_format* format2 = workbook_add_format(workbook);
                format_set_bold(format2);
                format_set_font_size(format2, 16);
                format_set_bg_color(format2, GRAY);
                format_set_align(format2, LXW_ALIGN_RIGHT);
                worksheet_merge_range(sheet, 1, 6, 1, 10, txt2, format2);
            }

            // Row 3 is all empty
            // Row 4 and onwards: desks
            int xrow = 3;
            lxw_format* alignTopCenter = workbook_add_format(workbook);
            format_set_align(alignTopCenter, LXW_ALIGN_VERTICAL_TOP);
            format_set_align(alignTopCenter, LXW_ALIGN_CENTER);

            for (int hrow = 0; hrow < hall.layout.rowCount; hrow++) {
                worksheet_set_row(sheet, xrow, 100, alignTopCenter);
                worksheet_set_row(sheet, xrow + 1, 10, nullptr);

                for (int hDoubleCol = 0; hDoubleCol < 3; hDoubleCol++) {
                    int hcol1 = hDoubleCol * 2;
                    int hcol2 = hcol1 + 1;

                    DeskCoordinates coord1(&hall, hrow, hcol1);
                    DeskCoordinates coord2(&hall, hrow, hcol2);

                    bool desk1Exists = createDesk(workbook, sheet, coord1);
                    bool desk2Exists = createDesk(workbook, sheet, coord2);

                    bool bothDesksEmpty = !(desk1Exists || desk2Exists);
                    int xcol = 2 * hcol1 + 1;
                    separator(workbook, sheet, xrow, xcol, bothDesksEmpty);
                }
                xrow += 2;
            }

            xrow++;

            QMap<int, int> gradePopulations;
            QList<QString> sections;
            for (const auto& [className, _] : scheme.classLists) {
                auto tokens = className.split("-");
                int grade = tokens.at(0).toInt();
                if (!gradePopulations.contains(grade)) {
                    gradePopulations.insert(grade, 0);
                }

                QString section = tokens.at(1);
                if (!sections.contains(section)) {
                    sections << section;
                }
            }
            std::sort(sections.begin(), sections.end());

            lxw_format* fmtInfoHeader = workbook_add_format(workbook);
            format_set_bold(fmtInfoHeader);
            format_set_font_size(fmtInfoHeader, 14);
            format_set_align(fmtInfoHeader, LXW_ALIGN_CENTER);
            format_set_bottom(fmtInfoHeader, LXW_BORDER_THIN);
            format_set_bg_color(fmtInfoHeader, LIGHT_GRAY);

            // Info panel headers
            {
                {
                    // First cell
                    lxw_format* format = workbook_add_format(workbook);
                    format_set_bottom(format, LXW_BORDER_THIN);
                    worksheet_write_string(sheet, xrow, 0, "", format);
                }

                int col = 1;
                for (auto it = gradePopulations.constBegin(); it != gradePopulations.constEnd(); ++it) {
                    int grade = it.key();
                    char* txt = QString("%1. Sınıf").arg(grade).toUtf8().data();

                    worksheet_merge_range(sheet, xrow, col, xrow, col + 1, txt, fmtInfoHeader);
                    col += 2;
                }
            }
            xrow++;

            // Info panel body
            {
                for (QString& section : sections) {
                    {
                        // First cell
                        char* txt = QString("%1 Şubesi").arg(section).toUtf8().data();
                        worksheet_write_string(sheet, xrow, 0, txt, fmtInfoHeader);
                    }

                    lxw_format* format = workbook_add_format(workbook);
                    format_set_bottom(format, LXW_BORDER_THIN);
                    format_set_align(format, LXW_ALIGN_CENTER);

                    int col = 1;
                    for (auto it = gradePopulations.constBegin(); it != gradePopulations.constEnd(); ++it) {
                        int grade = it.key();
                        int count = hall.countStudents(grade, section);
                        gradePopulations[grade] += count;
                        char* txt = QString::number(count).toUtf8().data();

                        worksheet_merge_range(sheet, xrow, col, xrow, col + 1, txt, format);
                        col += 2;
                    }

                    xrow++;
                }
            }

            // Info panel last row
            {
                int totalPopulation = 0;

                // First cell
                worksheet_write_string(sheet, xrow, 0, "Bütün Şubeler", fmtInfoHeader);

                // Population cells
                int col = 1;
                for (int count : std::as_const(gradePopulations)) {
                    totalPopulation += count;
                    char* txt = QString::number(count).toUtf8().data();

                    worksheet_merge_range(sheet, xrow, col, xrow, col + 1, txt, fmtInfoHeader);
                    col += 2;
                }

                // Last cell
                char* txt = QString("Toplam: %1").arg(totalPopulation).toUtf8().data();
                worksheet_merge_range(sheet, xrow, col, xrow, col + 1, txt, fmtInfoHeader);
            }

            worksheet_fit_to_pages(sheet, 1, 1);
            worksheet_center_horizontally(sheet);
            worksheet_set_portrait(sheet);
            worksheet_set_paper(sheet, PAPER_A4);
            worksheet_set_margins(sheet, 0.24, 0.24, 0.24, 0.24); // 0.24 inch ~= 0.6 cm
        }

        workbook_close(workbook);
    }

    void SchemeExporter::exportAll()
    {
        exportClassLists();
        exportHallLayouts();
        emit exportFinished(pathClassLists, pathHallLayouts);
    }

} // namespace BLL
} // namespace ikoOSKAR
