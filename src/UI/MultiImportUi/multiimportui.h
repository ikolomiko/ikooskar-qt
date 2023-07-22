#ifndef IKOOSKAR_UI_MULTIIMPORTUI_H
#define IKOOSKAR_UI_MULTIIMPORTUI_H

#include "Shared/student.h"
#include <QDialog>
#include <QList>

namespace ikoOSKAR {
namespace UI {
    using namespace Shared;

    namespace Ui {
        class MultiImportUi;
    }

    class MultiImportUi : public QDialog {
        Q_OBJECT

    public:
        explicit MultiImportUi(QWidget* parent = nullptr);
        ~MultiImportUi();

    public slots:
        void handleParsedXls(QList<Student*>*);
        void handleGradeAndSection(int grade, const QString& section);
        void handleConfirmation();

    private slots:
        void btnOpenFile_clicked();
        void btnHelpExcel_clicked();

    private:
        enum PageState {
            FILE_PICKER = 0,
            SPINNER,
            CLASS_PICKER,
            PREVIEW
        };

        Ui::MultiImportUi* ui;
        PageState page;
        QList<Student*>* parsedStudents;

        const QString* pickXlsFile();
        void prevPage();
        void nextPage();
    };

} // namespace UI
} // namespace ikoOSKAR
#endif // IKOOSKAR_UI_MULTIIMPORTUI_H
