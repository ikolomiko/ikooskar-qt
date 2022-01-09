#ifndef IKOOSKAR_UI_ERRORUI_H
#define IKOOSKAR_UI_ERRORUI_H
#include <QMessageBox>

namespace ikoOSKAR {
namespace UI {

    typedef std::function<void(const QString&)> _curry;
    // typedef void (*_curry)(QString);

    void ErrorUi(const QString& title, const QString& message);
    _curry ErrorUi(const QString& title);

} // namespace UI
} // namespace ikoOSKAR

#endif // IKOOSKAR_UI_ERRORUI_H
