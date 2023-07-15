#ifndef IKOOSKAR_UI_ERRORUI_H
#define IKOOSKAR_UI_ERRORUI_H
#include <QMessageBox>

namespace ikoOSKAR {
namespace UI {

    /// @brief A class to display error messages with a previously determined title
    class ErrorUi : QObject {
        Q_OBJECT

        /// @brief Contains the common title of the error messages
        const QString* const m_title;

    public:
        ErrorUi(const QString& title);
        void DisplayMessage(const QString& message);
    };

} // namespace UI
} // namespace ikoOSKAR

#endif // IKOOSKAR_UI_ERRORUI_H
