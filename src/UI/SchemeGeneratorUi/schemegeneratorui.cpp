#include "schemegeneratorui.h"
#include "ui_schemegeneratorui.h"

namespace ikoOSKAR {
namespace UI {

    SchemeGeneratorUi::SchemeGeneratorUi(QWidget* parent)
        : Common::Module(parent)
        , ui(new Ui::SchemeGeneratorUi)
    {
        ui->setupUi(this);
        name = new QString("Yeni Sınav Düzeni");
    }

    SchemeGeneratorUi::~SchemeGeneratorUi()
    {
        delete ui;
    }

    SchemeGeneratorUi* SchemeGeneratorUi::getInstance()
    {
        if (instance == nullptr) {
            instance = new SchemeGeneratorUi();
        }
        return instance;
    }

    const QString* SchemeGeneratorUi::getDescription()
    {
        return new QString("Kalan deneme hakkınız: x | Sınava katılacak öğrenci sayısı: y");
    }
} // namespace UI
} // namespace ikoOSKAR
