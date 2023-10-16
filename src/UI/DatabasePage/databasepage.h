#ifndef IKOOSKAR_UI_DATABASEPAGE_H
#define IKOOSKAR_UI_DATABASEPAGE_H

#include "BLL/DatabaseHelper/databasehelper.h"
#include "UI/Common/page.h"
#include <QTableWidget>
#include <QWidget>

namespace ikoOSKAR {
namespace UI {
    namespace Ui {
        class DatabasePage;
    }

    class DatabasePage : public Common::Page {
        Q_OBJECT

    public:
        class ClassTable : public QTableWidget {
        public:
            ClassTable(QList<Student*>* students);
        };

        static DatabasePage* getInstance();
        const QString* getName();
        const QString* getDescription();
        ~DatabasePage();

    public slots:
        void actionAddSingle_clicked();

        void actionAddMulti_clicked();

        void actionEoty_clicked();

        void actionRemoveClass_clicked();

    private slots:
        void handleError(const QString& errorMessage);
        
        void on_btnDelete_clicked();

        void on_btnEdit_clicked();

    private:
        void refresh();
        void createButtonMenus();
        void createTabWidget();
        Ui::DatabasePage* ui;
        ikoOSKAR::BLL::DatabaseHelper* bll;
        inline static DatabasePage* instance;
        explicit DatabasePage(QWidget* parent = nullptr);
    };
} // namespace UI
} // namespace ikoOSKAR
#endif // IKOOSKAR_UI_DATABASEPAGE_H
