#ifndef IKOOSKAR_UI_MAINPAGE_H
#define IKOOSKAR_UI_MAINPAGE_H

#include <QMainWindow>

namespace ikoOSKAR {
namespace UI {

namespace Ui {
class MainPage;
}

class MainPage : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainPage(QWidget *parent = nullptr);
    ~MainPage();

private:
    Ui::MainPage *ui;
};


} // namespace UI
} // namespace ikoOSKAR
#endif // IKOOSKAR_UI_MAINPAGE_H
