#include "historyui.h"
#include "ui_historyui.h"

namespace ikoOSKAR {
namespace UI {

    HistoryUi::HistoryUi(QWidget* parent)
        : Common::Module(parent)
        , ui(new Ui::HistoryUi)
    {
        ui->setupUi(this);
        name = new QString("Önceki Sınav Düzenleri");
    }

    HistoryUi::~HistoryUi()
    {
        delete ui;
    }

    HistoryUi* HistoryUi::getInstance()
    {
        if (instance == nullptr) {
            instance = new HistoryUi();
        }
        return instance;
    }

    const QString* HistoryUi::getDescription()
    {
        return new QString("Toplam yapılan sınav sayısı: x");
    }

} // namespace UI
} // namespace ikoOSKAR
