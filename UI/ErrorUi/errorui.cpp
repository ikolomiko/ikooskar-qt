#include "errorui.h"
#include <QApplication>

namespace ikoOSKAR {
namespace UI {

    /**
     * @brief Initializes a new instance of ErrorUi with the given title
     * @param title : The title to be displayed on error messages
     */
    ErrorUi::ErrorUi(const QString& title)
        : m_title(&title)
    {
    }

    /**
     * @brief Displays an error message with the previously determined title
     * @param message : The message to be displayed on the error message
     */
    void ErrorUi::DisplayMessage(const QString& message)
    {
        QMessageBox::critical(QApplication::activeWindow(), *m_title, message);
    }

} // namespace UI
} // namespace ikoOSKAR
