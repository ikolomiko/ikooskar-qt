#include "errorui.h"
#include <QApplication>

namespace ikoOSKAR {
namespace UI {

    // usage: ikoOSKAR::UI::ErrorUi(title, message)
    void ErrorUi(const QString& title, const QString& message)
    {
        QMessageBox::critical(QApplication::activeWindow(), title, message);
    }

    // usage: ikoOSKAR::UI::ErrorUi(title)(message)
    _curry ErrorUi(const QString& title)
    {
        return [&](const QString& message) { ErrorUi(title, message); };
    }

} // namespace UI
} // namespace ikoOSKAR
