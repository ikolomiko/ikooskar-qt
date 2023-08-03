#ifndef IKOOSKAR_UI_NSPREVIEWUI_H
#define IKOOSKAR_UI_NSPREVIEWUI_H

#include "BLL/SchemeGenerator/schemegenerator.h"
#include <QWidget>

using Preview = ikoOSKAR::BLL::SchemeGenerator::Preview;

namespace ikoOSKAR {
namespace UI {

    namespace Ui {
        class NSPreviewUi;
    }

    class NSPreviewUi : public QWidget {
        Q_OBJECT

    public:
        explicit NSPreviewUi(Preview preview, QWidget* parent = nullptr);
        ~NSPreviewUi();

    private:
        Ui::NSPreviewUi* ui;
    };

} // namespace UI
} // namespace ikoOSKAR
#endif // IKOOSKAR_UI_NSPREVIEWUI_H
