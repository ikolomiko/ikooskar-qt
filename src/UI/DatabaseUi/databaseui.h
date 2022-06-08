#ifndef IKOOSKAR_UI_DATABASEUI_H
#define IKOOSKAR_UI_DATABASEUI_H

#include "BLL/DatabaseHelper/databasehelper.h"
#include <QWidget>

namespace ikoOSKAR {
namespace UI {
    namespace Ui {
        class DatabaseUi;
    }

    class DatabaseUi : public QWidget {
        Q_OBJECT

    public:
        static DatabaseUi* getInstance();
        ~DatabaseUi();

    private slots:
        void on_btnAdd_clicked();

    private:
        Ui::DatabaseUi* ui;
        ikoOSKAR::BLL::DatabaseHelper* bll;
        inline static DatabaseUi* instance;
        explicit DatabaseUi(QWidget* parent = nullptr);
    };
} // namespace UI
} // namespace ikoOSKAR
#endif // IKOOSKAR_UI_DATABASEUI_H
