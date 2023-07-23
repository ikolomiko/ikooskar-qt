#ifndef IKOOSKAR_UI_STUDENTEDITORUI_H
#define IKOOSKAR_UI_STUDENTEDITORUI_H

#include "Shared/student.h"
#include <QDialog>

namespace ikoOSKAR {
namespace UI {

    namespace Ui {
        class StudentEditorUi;
    }

    class StudentEditorUi : public QDialog {
        Q_OBJECT

    public:
        enum EditorMode : int { ADD,
            EDIT };
        explicit StudentEditorUi(EditorMode mode, QWidget* parent = nullptr, Shared::Student* student = nullptr);
        ~StudentEditorUi();

    private slots:
        void handleError(const QString& errorMessage);
        void done(int r);

    private:
        Ui::StudentEditorUi* ui;
        Shared::Student* student;
        EditorMode mode;
    };

} // namespace UI
} // namespace ikoOSKAR
#endif // IKOOSKAR_UI_STUDENTEDITORUI_H
