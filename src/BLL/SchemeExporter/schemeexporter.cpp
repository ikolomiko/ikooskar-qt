#include "schemeexporter.h"
#include "xlsxwriter.h"
#include <QColor>
#include <QDir>

#define GRAY (0xaaaaaa)
#define LIGHT_GRAY (0xd3d3d3)
#define PAPER_A4 (9)

namespace ikoOSKAR {
namespace BLL {

    SchemeExporter::SchemeExporter(Scheme scheme)
        : scheme(scheme)
    {
        QDir().mkpath(scheme.path());
    }

    void SchemeExporter::exportClassLists()
    {
        const auto filePath = scheme.path() + "/Sınıf Karma Listeleri.xlsx";
        lxw_workbook* workbook = workbook_new(filePath.toUtf8());

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
    }

    void SchemeExporter::exportAll()
    {
        exportClassLists();
        exportHallLayouts();
    }

} // namespace BLL
} // namespace ikoOSKAR
