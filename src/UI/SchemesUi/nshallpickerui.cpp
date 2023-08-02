#include "nshallpickerui.h"
#include "BLL/DatabaseHelper/databasehelper.h"
#include "ui_nshallpickerui.h"

namespace ikoOSKAR {
namespace UI {

    NSHallPickerUi::NSHallPickerUi(QWidget* parent)
        : QWidget(parent)
        , ui(new Ui::NSHallPickerUi)
        , includedModel(new QStringListModel)
        , excludedModel(new QStringListModel)
    {
        ui->setupUi(this);

        auto excludedList = BLL::DatabaseHelper::getInstance()->GetAllHallNames();
        excludedList.sort();
        QList<QString> includedList;

        // Restore previously selected halls
        QSettings settings;
        int size = settings.beginReadArray("selected_hallnames");
        for (int i = 0; i < size; i++) {
            settings.setArrayIndex(i);
            QString hallName = settings.value("hallname").toString();
            if (excludedList.removeAll(hallName)) {
                includedList.append(hallName);
            }
        }
        settings.endArray();

        includedModel->setStringList(includedList);
        excludedModel->setStringList(excludedList);

        ui->lsIncluded->setModel(includedModel);
        ui->lsExcluded->setModel(excludedModel);
    }

    NSHallPickerUi::~NSHallPickerUi()
    {
        updateSelectedHalls();
        delete ui;
    }

    QList<QString> NSHallPickerUi::getSelectedHalls()
    {
        return includedModel->stringList();
    }

    void NSHallPickerUi::on_btnAddHall_clicked()
    {
        auto selectedIndexes = ui->lsExcluded->selectionModel()->selectedIndexes();
        if (selectedIndexes.isEmpty()) {
            return;
        }

        QStringList selectedHalls;
        for (const auto idx : selectedIndexes) {
            selectedHalls << excludedModel->data(idx, Qt::DisplayRole).toString();
        }

        // Remove selected items from excluded
        for (const QString& item : selectedHalls) {
            int index = excludedModel->stringList().indexOf(item);
            excludedModel->removeRow(index);
        }

        // Add selected items to included
        QStringList existingItems = includedModel->stringList();
        existingItems.append(selectedHalls);
        existingItems.sort();
        includedModel->setStringList(existingItems);
    }

    void NSHallPickerUi::on_btnRmHall_clicked()
    {
        auto selectedIndexes = ui->lsIncluded->selectionModel()->selectedIndexes();
        if (selectedIndexes.isEmpty()) {
            return;
        }

        QStringList selectedHalls;
        for (const auto idx : selectedIndexes) {
            selectedHalls << includedModel->data(idx, Qt::DisplayRole).toString();
        }

        // Remove selected items from included
        for (const QString& item : selectedHalls) {
            int index = includedModel->stringList().indexOf(item);
            includedModel->removeRow(index);
        }

        // Add selected items to excluded
        QStringList existingItems = excludedModel->stringList();
        existingItems.append(selectedHalls);
        existingItems.sort();
        excludedModel->setStringList(existingItems);
    }

    void NSHallPickerUi::updateSelectedHalls()
    {
        auto selectedHalls = includedModel->stringList();

        QSettings settings;
        settings.beginWriteArray("selected_hallnames");
        for (int i = 0; i < selectedHalls.size(); i++) {
            settings.setArrayIndex(i);
            settings.setValue("hallname", selectedHalls.at(i));
        }
        settings.endArray();
    }

} // namespace UI
} // namespace ikoOSKAR
