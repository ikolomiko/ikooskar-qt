#include "spinner.h"
#include "ui_spinner.h"

namespace ikoOSKAR {
namespace UI {
    namespace Common {

        Spinner::Spinner(QWidget* parent)
            : QWidget(parent)
            , ui(new Ui::Spinner)
        {
            ui->setupUi(this);
            spinnerGif = new QMovie(":/spinner.gif");
            spinnerGif->setScaledSize({ 100, 100 });
            spinnerGif->setSpeed(200);
            ui->lblSpinner->setMovie(spinnerGif);
        }

        void Spinner::setTitle(const QString& title)
        {
            ui->lblHeader->setText(title);
        }

        void Spinner::start()
        {
            spinnerGif->start();
        }

        void Spinner::stop()
        {
            spinnerGif->stop();
        }

        Spinner::~Spinner()
        {
            delete spinnerGif;
            delete ui;
        }

    } // namespace Common
} // namespace UI
} // namespace ikoOSKAR
