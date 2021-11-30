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
    void on_btnHome_clicked();

    void on_btnDatabase_clicked();

    void on_btnNewScheme_clicked();

    void on_btnHistory_clicked();

    void on_btnHelp_clicked();

private:
    Ui::MainPage *ui;
    void changePage(int, QWidget*);
};


} // namespace UI
} // namespace ikoOSKAR
#endif // IKOOSKAR_UI_MAINPAGE_H
