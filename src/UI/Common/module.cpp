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

        void Module::setDescriptionLabel(QLabel* lbl)
        {
            lblDescription = lbl;
        }
    } // namespace Common
} // namespace UI
} // namespace ikoOSKAR
