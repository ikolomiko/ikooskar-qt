#ifndef IKOOSKAR_UI_DATABASEUI_H
#define IKOOSKAR_UI_DATABASEUI_H

#include "../Common/module.h"
#include "BLL/DatabaseHelper/databasehelper.h"
#include <QWidget>

namespace ikoOSKAR {
namespace UI {
    namespace Ui {
        class DatabaseUi;
    }

    class DatabaseUi : public Common::Module {
        Q_OBJECT

    public:
        static DatabaseUi* getInstance();
        const QString* getName();
        const QString* getDescription();
        ~DatabaseUi();

    public slots:
        void actionAddSingle_clicked();

        void actionAddMulti_clicked();

    private slots:
        void on_btnAdd_clicked();

        void on_btnDelete_clicked();

    private:
        Ui::DatabaseUi* ui;
        ikoOSKAR::BLL::DatabaseHelper* bll;
        void ReloadTableData();
        inline static DatabaseUi* instance;
        explicit DatabaseUi(QWidget* parent = nullptr);
    };
} // namespace UI
} // namespace ikoOSKAR
#endif // IKOOSKAR_UI_DATABASEUI_H
