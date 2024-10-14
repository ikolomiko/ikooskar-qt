#ifndef IKOOSKAR_UI_MAINWINDOW_H
#define IKOOSKAR_UI_MAINWINDOW_H

#include "BLL/Authenticator/authenticator.h"
#include <QMainWindow>
#include <QPushButton>

namespace ikoOSKAR {
namespace UI {

    namespace Ui {
        class MainWindow;
    }

    class MainWindow : public QMainWindow {
        Q_OBJECT

    public:
        explicit MainWindow(QWidget* parent = nullptr);
        ~MainWindow();

    public slots:
        void setDescription(const QString& description);

    private:
        enum Subpage : int;
        Ui::MainWindow* ui;
        BLL::Authenticator* authenticator;

        void initSubpages();
        void changePage(Subpage index, QIcon icon);
        void refreshDemoStatus();

    signals:
        void restartApp();
    };

} // namespace UI
} // namespace ikoOSKAR
#endif // IKOOSKAR_UI_MAINWINDOW_H
