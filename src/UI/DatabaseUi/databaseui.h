#ifndef IKOOSKAR_UI_DATABASEUI_H
#define IKOOSKAR_UI_DATABASEUI_H

#include "BLL/DatabaseHelper/databasehelper.h"
#include "UI/Common/module.h"
#include <QTableWidget>
#include <QWidget>

namespace ikoOSKAR {
namespace UI {
    namespace Ui {
        class DatabaseUi;
    }

    class DatabaseUi : public Common::Module {
        Q_OBJECT

    public:
        class ClassTable : public QTableWidget {
        public:
            ClassTable(QList<Student*>* students);
        };

        static DatabaseUi* getInstance();
        const QString* getName();
        const QString* getDescription();
        ~DatabaseUi();

    public slots:
        void actionAddSingle_clicked();

        void actionAddMulti_clicked();

        void actionEoty_clicked();

        void actionRemoveClass_clicked();

    private slots:
        void on_btnDelete_clicked();

        void on_btnEdit_clicked();

    private:
        void refresh();
        void createButtonMenus();
        void createTabWidget();
        Ui::DatabaseUi* ui;
        ikoOSKAR::BLL::DatabaseHelper* bll;
        inline static DatabaseUi* instance;
        explicit DatabaseUi(QWidget* parent = nullptr);
    };
} // namespace UI
} // namespace ikoOSKAR
#endif // IKOOSKAR_UI_DATABASEUI_H
