#ifndef IKOOSKAR_UI_STUDENTEDITORDIALOG_H
#define IKOOSKAR_UI_STUDENTEDITORDIALOG_H

#include "Shared/student.h"
#include <QDialog>

namespace ikoOSKAR {
namespace UI {

    namespace Ui {
        class StudentEditorDialog;
    }

    class StudentEditorDialog : public QDialog {
        Q_OBJECT

    public:
        enum EditorMode : int { ADD,
            EDIT };
        explicit StudentEditorDialog(EditorMode mode, QWidget* parent = nullptr, Shared::Student* student = nullptr);
        ~StudentEditorDialog();

    private slots:
        void handleError(const QString& errorMessage);
        void done(int r);

    private:
        Ui::StudentEditorDialog* ui;
        Shared::Student* student;
        EditorMode mode;
    };

} // namespace UI
} // namespace ikoOSKAR
#endif // IKOOSKAR_UI_STUDENTEDITORDIALOG_H
