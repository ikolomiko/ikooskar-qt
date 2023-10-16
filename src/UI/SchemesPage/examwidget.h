/*
    Elypson/qt-collapsible-section
    (c) 2016 Michael A. Voelkel - michael.alexander.voelkel@gmail.com

    This file is part of Elypson/qt-collapsible section.

    Elypson/qt-collapsible-section is free software: you can redistribute it and/or modify
    it under the terms of the GNU Lesser General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    Elypson/qt-collapsible-section is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU Lesser General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with Elypson/qt-collapsible-section. If not, see <http://www.gnu.org/licenses/>.
*/

#ifndef IKOOSKAR_UI_COLLAPSIBLE_H
#define IKOOSKAR_UI_COLLAPSIBLE_H

#include "Shared/scheme.h"
#include <QFrame>
#include <QGridLayout>
#include <QParallelAnimationGroup>
#include <QPushButton>
#include <QScrollArea>
#include <QToolButton>
#include <QWidget>

namespace ikoOSKAR {
namespace UI {

    class ExamWidget : public QFrame {
        Q_OBJECT

    private:
        QGridLayout* mainLayout;
        QToolButton* btnToggle;
        QPushButton* btnFolder;
        QParallelAnimationGroup* toggleAnimation;
        QWidget* contentArea;
        int animationDuration;
        int collapsedHeight;
        bool isExpanded = false;
        Shared::Scheme exam;
        QIcon arrowRight, arrowDown;

    public slots:
        void toggle(bool collapsed);

    public:
        explicit ExamWidget(const Shared::Scheme& exam, const int animationDuration = 0, QWidget* parent = 0);

        void setContentLayout(QLayout& contentLayout);

        void setTitle(QString title);

        void updateHeights();

        void btnFolderOnClick();
    };

} // namespace UI
} // namespace ikoOSKAR

#endif // IKOOSKAR_UI_COLLAPSIBLE_H
