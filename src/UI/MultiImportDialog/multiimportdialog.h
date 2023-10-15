#ifndef IKOOSKAR_UI_MULTIIMPORTDIALOG_H
#define IKOOSKAR_UI_MULTIIMPORTDIALOG_H

#include "Shared/student.h"
#include <QDialog>
#include <QList>

namespace ikoOSKAR {
namespace UI {
    using namespace Shared;

    namespace Ui {
        class MultiImportDialog;
    }

    class MultiImportDialog : public QDialog {
        Q_OBJECT

    public:
        explicit MultiImportDialog(QWidget* parent = nullptr);
        ~MultiImportDialog();

    private slots:
        void handleXlsPath(QString* xlsFilePath);
        void handleParsedXls(QList<Student*>*);
        void handleGradeAndSection(int grade, const QString& section);
        void handleConfirmation();
        void handleError(const QString& errorMessage);

    private:
        enum PageState {
            FILE_PICKER = 0,
            SPINNER,
            CLASS_PICKER,
            PREVIEW
        };

        Ui::MultiImportDialog* ui;
        PageState page;
        QList<Student*>* parsedStudents;

        void prevPage();
        void nextPage();
    };

} // namespace UI
} // namespace ikoOSKAR
#endif // IKOOSKAR_UI_MULTIIMPORTDIALOG_H
