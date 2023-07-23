#ifndef IKOOSKAR_UI_MAINPAGE_H
#define IKOOSKAR_UI_MAINPAGE_H

#include <QMainWindow>
#include <QPushButton>

namespace ikoOSKAR {
namespace UI {

    namespace Ui {
        class MainPage;
    }

    class MainPage : public QMainWindow {
        Q_OBJECT

    public:
        explicit MainPage(QWidget* parent = nullptr);
        ~MainPage();

    public slots:
        void setDescription(const QString& description);

    private:
        enum Subpage : int;
        Ui::MainPage* ui;
        void changePage(Subpage index, QIcon icon);
    };

} // namespace UI
} // namespace ikoOSKAR
#endif // IKOOSKAR_UI_MAINPAGE_H
