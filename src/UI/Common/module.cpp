#include "module.h"

namespace ikoOSKAR {
namespace UI {
    namespace Common {

        Module::Module(QWidget* parent)
            : QWidget(parent)
        {
        }

        const QString* Module::getName()
        {
            return name;
        }
    } // namespace Common
} // namespace UI
} // namespace ikoOSKAR
