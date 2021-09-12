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
    
private slots:
    void on_btn_database_clicked();

    void on_btn_newscheme_clicked();

    void on_btn_history_clicked();

    void on_btn_help_clicked();
    
private:
    Ui::MainPage *ui;
};


} // namespace UI
} // namespace ikoOSKAR
#endif // IKOOSKAR_UI_MAINPAGE_H
