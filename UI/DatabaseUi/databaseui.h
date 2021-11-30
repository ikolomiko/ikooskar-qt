#ifndef IKOOSKAR_UI_DATABASEUI_H
#define IKOOSKAR_UI_DATABASEUI_H

#include <QWidget>

namespace ikoOSKAR {
namespace UI {

namespace Ui {
class DatabaseUi;
}

class DatabaseUi : public QWidget
{
    Q_OBJECT

public:
    explicit DatabaseUi(QWidget *parent = nullptr);
    ~DatabaseUi();

private:
    Ui::DatabaseUi *ui;
};


} // namespace UI
} // namespace ikoOSKAR
#endif // IKOOSKAR_UI_DATABASEUI_H
