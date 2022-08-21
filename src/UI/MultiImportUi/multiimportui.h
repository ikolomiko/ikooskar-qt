#ifndef IKOOSKAR_UI_MULTIIMPORTUI_H
#define IKOOSKAR_UI_MULTIIMPORTUI_H

#include <QDialog>

namespace ikoOSKAR {
namespace UI {

namespace Ui {
class MultiImportUi;
}

class MultiImportUi : public QDialog
{
    Q_OBJECT

public:
    explicit MultiImportUi(QWidget *parent = nullptr);
    ~MultiImportUi();

private:
    Ui::MultiImportUi *ui;
};


} // namespace UI
} // namespace ikoOSKAR
#endif // IKOOSKAR_UI_MULTIIMPORTUI_H
