#ifndef MAINPAGE_H
#define MAINPAGE_H

#include <QMainWindow>

QT_BEGIN_NAMESPACE
namespace Ui { class MainPage; }
QT_END_NAMESPACE

class MainPage : public QMainWindow
{
    Q_OBJECT

public:
    MainPage(QWidget *parent = nullptr);
    ~MainPage();

private:
    Ui::MainPage *ui;
};
#endif // MAINPAGE_H
