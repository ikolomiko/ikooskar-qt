#include "nsclasspickerui.h"
#include "BLL/DatabaseHelper/databasehelper.h"
#include "ui_nsclasspickerui.h"

namespace ikoOSKAR {
namespace UI {

    NSClassPickerUi::NSClassPickerUi(QWidget* parent)
        : QWidget(parent)
        , ui(new Ui::NSClassPickerUi)
        , attendingModel(new QStringListModel)
        , nonAttendingModel(new QStringListModel)
    {
        ui->setupUi(this);

        auto nonAttendingList = *BLL::DatabaseHelper::getInstance()->GetClassNames();
        nonAttendingList.sort();
        QList<QString> attendingList;

        // Restore previously selected classes
        QSettings settings;
        int size = settings.beginReadArray("selected_classnames");
        for (int i = 0; i < size; i++) {
            settings.setArrayIndex(i);
            QString className = settings.value("classname").toString();
            if (nonAttendingList.removeAll(className) != 0) {
                attendingList.append(className);
            }
        }
        settings.endArray();

        attendingModel->setStringList(attendingList);
        nonAttendingModel->setStringList(nonAttendingList);

        ui->lsAttending->setModel(attendingModel);
        ui->lsNotAttending->setModel(nonAttendingModel);
    }

    NSClassPickerUi::~NSClassPickerUi()
    {
        updateSelectedClasses();
        delete ui;
    }

    QList<QString> NSClassPickerUi::getAttendingClasses()
    {
        return attendingModel->stringList();
    }

    void NSClassPickerUi::on_btnAddClass_clicked()
    {
        auto selectedIndexes = ui->lsNotAttending->selectionModel()->selectedIndexes();
        if (selectedIndexes.isEmpty()) {
            return;
        }

        QStringList selectedClasses;
        for (const auto idx : selectedIndexes) {
            selectedClasses << nonAttendingModel->data(idx, Qt::DisplayRole).toString();
        }

        // Remove selected items from nonAttending
        for (const QString& item : selectedClasses) {
            int index = nonAttendingModel->stringList().indexOf(item);
            nonAttendingModel->removeRow(index);
        }

        // Add selected items to attending
        QStringList existingItems = attendingModel->stringList();
        existingItems.append(selectedClasses);
        existingItems.sort();
        attendingModel->setStringList(existingItems);
    }

    void NSClassPickerUi::on_btnRmClass_clicked()
    {
        auto selectedIndexes = ui->lsAttending->selectionModel()->selectedIndexes();
        if (selectedIndexes.isEmpty()) {
            return;
        }

        QStringList selectedClasses;
        for (const auto idx : selectedIndexes) {
            selectedClasses << attendingModel->data(idx, Qt::DisplayRole).toString();
        }

        // Remove selected items from attending
        for (const QString& item : selectedClasses) {
            int index = attendingModel->stringList().indexOf(item);
            attendingModel->removeRow(index);
        }

        // Add selected items to nonAttending
        QStringList existingItems = nonAttendingModel->stringList();
        existingItems.append(selectedClasses);
        existingItems.sort();
        nonAttendingModel->setStringList(existingItems);
    }

    void NSClassPickerUi::updateSelectedClasses()
    {
        auto selectedClasses = attendingModel->stringList();

        QSettings settings;
        settings.beginWriteArray("selected_classnames");
        for (int i = 0; i < selectedClasses.size(); i++) {
            settings.setArrayIndex(i);
            settings.setValue("classname", selectedClasses.at(i));
        }
        settings.endArray();
    }

} // namespace UI
} // namespace ikoOSKAR
