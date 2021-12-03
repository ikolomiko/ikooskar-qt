#include "errorui.h"
#include <QApplication>

namespace ikoOSKAR {
namespace UI {

// usage: ikoOSKAR::UI::ErrorUi(title, message)
void ErrorUi(QString title, QString message)
{
    QMessageBox::critical(QApplication::activeWindow(), title, message);
}

// usage: ikoOSKAR::UI::ErrorUi(title)(message)
_curry ErrorUi(QString title){
    return [title](QString message){
        ErrorUi(title, message);
    };
}

} // namespace UI
} // namespace ikoOSKAR
