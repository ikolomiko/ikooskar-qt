#include "hall.h"
#include <QJsonArray>
#include <cmath>

namespace ikoOSKAR {
namespace Shared {

    Hall::Layout::Layout() { }

    Hall::Layout::Layout(int capacity)
    {
        rowCount = std::ceil(capacity / 6);
        desks = new Desk**[rowCount];

        // Fill rowCount * 6 desks
        for (int row = 0; row < rowCount; row++) {
            desks[row] = new Desk*[6];
            for (int col = 0; col < 6; col++) {
                desks[row][col] = new Desk();
            }
        }

        // Delete the empty (non existent) desks from the last row
        int col = capacity % 6;
        if (col == 0)
            return;

        for (; col < 6; col++) {
            delete desks[rowCount - 1][col];
            desks[rowCount - 1][col] = nullptr;
        }
    }

    Hall::Layout Hall::Layout::fromJson(const QJsonObject& json)
    {
        Layout result;

        if (const QJsonValue v = json["rowCount"]; v.isDouble()) {
            result.rowCount = v.toInt();
        }

        result.desks = new Desk**[result.rowCount];
        if (const QJsonValue v = json["desks"]; v.isArray()) {
            QJsonArray jDesks = v.toArray();
            for (int row = 0; row < jDesks.size(); row++) {
                result.desks[row] = new Desk*[6];
                QJsonArray jRow = jDesks.at(row).toArray();
                for (int col = 0; col < 6; col++) {
                    bool deskExists = jRow.at(col).toBool();
                    if (deskExists) {
                        result.desks[row][col] = new Desk();
                    } else {
                        result.desks[row][col] = nullptr;
                    }
                }
            }
        }

        return result;
    }

    QJsonObject Hall::Layout::toJson() const
    {
        QJsonObject result;
        result["rowCount"] = rowCount;

        // Convert the 2D desks pointer matrix into a json array of arrays
        // The values in the json array are booleans, indicating whether the desk exists (not a nullptr)
        QJsonArray jDesks;
        for (int row = 0; row < rowCount; row++) {
            QJsonArray jRow;
            for (int col = 0; col < 6; col++) {
                jRow.append(desks[row][col] != nullptr);
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

} // namespace Shared
} // namespace ikoOSKAR
