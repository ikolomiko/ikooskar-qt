#include "hall.h"
#include <QJsonArray>
#include <QJsonDocument>
#include <cmath>

namespace ikoOSKAR {
namespace Shared {

    Hall::Layout::Layout() { }

    Hall::Layout::Layout(int capacity)
    {
        int rowCount = std::ceil((float)capacity / 6);

        // Fill rowCount * 6 desks
        for (int i = 0; i < rowCount; i++) {
            DeskRow row;
            deskRows.push_back(row);
        }

        // Set the non-existent flag for the non-existent desks from the last row
        int col = capacity % 6;
        if (col == 0)
            return;

        for (; col < 6; col++) {
            deskRows[rowCount - 1][col].exists = false;
        }
    }

    Hall::Layout Hall::Layout::fromJsonStr(const QString& jsonStr)
    {
        auto jObject = QJsonDocument::fromJson(jsonStr.toUtf8()).object();
        return fromJson(jObject);
    }

    QString Hall::Layout::toJsonStr() const
    {
        auto jObject = toJson();
        return QJsonDocument(jObject).toJson(QJsonDocument::Compact);
    }

    void Hall::Layout::clear()
    {
        for (auto row : deskRows) {
            for (auto desk : row) {
                desk.isEmpty = true;
                desk.student = {};
            }
        }
    }

    Hall::Layout Hall::Layout::fromJson(const QJsonObject& json)
    {
        Layout result;

        if (const QJsonValue v = json["desks"]; v.isArray()) {
            QJsonArray jDesks = v.toArray();
            for (int rowIdx = 0; rowIdx < jDesks.size(); rowIdx++) {
                DeskRow row;
                QJsonArray jRow = jDesks.at(rowIdx).toArray();
                for (int colIdx = 0; colIdx < 6; colIdx++) {
                    bool deskExists = jRow.at(colIdx).toBool();
                    row[colIdx].exists = deskExists;
                }
                result.deskRows.push_back(row);
            }
        }

        return result;
    }

    QJsonObject Hall::Layout::toJson() const
    {
        QJsonObject result;

        // Convert the 2D desks pointer matrix into a json array of arrays
        // The values in the json array are booleans, indicating whether the desk exists
        QJsonArray jDesks;
        for (auto row : deskRows) {
            QJsonArray jRow;
            for (auto desk : row) {
                jRow.append(desk.exists);
            }
            jDesks.append(jRow);
        }
        result["desks"] = jDesks;

        return result;
    }

    Hall::Hall() { }

    Hall::Hall(const QString& name, int capacity)
        : name(name)
        , capacity(capacity)
        , layout(Layout(capacity))
    {
    }

    Hall::Hall(const QString& name, int capacity, const Layout& layout)
        : name(name)
        , capacity(capacity)
        , layout(layout)
    {
    }

    int Hall::countVariant(Pattern::Variant v)
    {
        if (pattern == nullptr) {
            return -1;
        }

        int count = 0;
        for (size_t rowIdx = 0; rowIdx < layout.deskRows.size(); rowIdx++) {
            for (size_t colIdx = 0; colIdx < 6; colIdx++) {
                const auto& desk = layout.deskRows[rowIdx][colIdx];
                if (desk.exists && pattern->variantAt(rowIdx, colIdx) == v) {
                    count++;
                }
            }
        }

        return count;
    }

    int Hall::countStudents(int grade, const QString& section)
    {
        int count = 0;
        for (auto row : layout.deskRows) {
            for (auto desk : row) {
                if (desk.exists && !desk.isEmpty && desk.student.has_value()) {
                    const auto& student = desk.student.value();
                    if (student.grade == grade && student.section == section) {
                        count++;
                    }
                }
            }
        }

        return count;
    }

} // namespace Shared
} // namespace ikoOSKAR
