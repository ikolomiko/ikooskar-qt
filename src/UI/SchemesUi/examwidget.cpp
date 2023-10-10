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

#include "examwidget.h"
#include "nsresultsui.h"
#include <QApplication>
#include <QDesktopServices>
#include <QPropertyAnimation>
#include <QStyleFactory>

namespace ikoOSKAR {
namespace UI {

    ExamWidget::ExamWidget(const Shared::Scheme& exam, const int animationDuration, QWidget* parent)
        : QFrame(parent)
        , animationDuration(animationDuration)
        , exam(exam)
    {
        setStyle(QStyleFactory::create("Fusion"));

        btnToggle = new QToolButton(this);
        btnFolder = new QPushButton(this);
        toggleAnimation = new QParallelAnimationGroup(this);
        contentArea = new QWidget();
        mainLayout = new QGridLayout(this);

        QPixmap pixmapArrow(":option-bullet.png");
        QTransform rotation;
        rotation.rotate(90);
        arrowRight = QIcon(pixmapArrow);
        arrowDown = QIcon(pixmapArrow.transformed(rotation));

        btnToggle->setStyleSheet("border: none; font-size: 17pt;");
        btnToggle->setToolButtonStyle(Qt::ToolButtonTextBesideIcon);
        btnToggle->setIconSize({ 30, 30 });
        btnToggle->setIcon(QIcon(":/option-bullet.png"));
        btnToggle->setText(" " + exam.name + "  -  " + exam.date);
        btnToggle->setCheckable(true);
        btnToggle->setChecked(false);
        btnToggle->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
        btnToggle->setCursor(QCursor(Qt::CursorShape::PointingHandCursor));
        btnToggle->setStyle(QApplication::style());
        connect(btnToggle, &QToolButton::toggled, this, &ExamWidget::toggle);

        btnFolder->setText("  Klasörü Aç  ");
        btnFolder->setIcon(QIcon(":/folder.png"));
        btnFolder->setMinimumHeight(50);
        btnFolder->setMaximumWidth(150);
        btnFolder->setIconSize({ 32, 32 });
        btnFolder->setVisible(false);
        btnFolder->setStyle(QApplication::style());
        connect(btnFolder, &QPushButton::clicked, this, &ExamWidget::btnFolderOnClick);

        contentArea->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);

        // start out collapsed
        contentArea->setMaximumHeight(0);
        contentArea->setMinimumHeight(0);
        contentArea->setStyle(QApplication::style());

        // let the entire widget grow and shrink with its content
        toggleAnimation->addAnimation(new QPropertyAnimation(this, "maximumHeight"));
        toggleAnimation->addAnimation(new QPropertyAnimation(this, "minimumHeight"));
        toggleAnimation->addAnimation(new QPropertyAnimation(contentArea, "maximumHeight"));

        mainLayout->setVerticalSpacing(0);
        mainLayout->setContentsMargins(9, 5, 9, 5);

        int row = 0;
        mainLayout->addWidget(btnToggle, row, 0, 1, 3, Qt::AlignLeft);
        mainLayout->addWidget(btnFolder, row++, 2, 1, 1);
        mainLayout->addWidget(contentArea, row, 0, 1, 3);
        setLayout(mainLayout);

        auto* vLayout = new QVBoxLayout();
        NSResultsUi* res = new NSResultsUi("", "", exam.classListPath(), exam.hallLayoutPath(), this, true);
        vLayout->setContentsMargins(0, 0, 0, 0);
        vLayout->setSpacing(0);
        vLayout->addWidget(res);
        setContentLayout(*vLayout);
    }

    void ExamWidget::toggle(bool expanded)
    {
        btnToggle->setIcon(expanded ? arrowDown : arrowRight);
        toggleAnimation->setDirection(expanded ? QAbstractAnimation::Forward : QAbstractAnimation::Backward);
        toggleAnimation->start();

        this->isExpanded = expanded;
        btnFolder->setVisible(expanded);

        if (expanded) {
            setFrameStyle(QFrame::StyledPanel | QFrame::Raised);
            btnToggle->setText("  " + exam.name + "\n  " + exam.date);
        } else {
            setFrameStyle(0);
            btnToggle->setText(" " + exam.name + "  -  " + exam.date);
        }
    }

    void ExamWidget::setContentLayout(QLayout& contentLayout)
    {
        delete contentArea->layout();
        contentArea->setLayout(&contentLayout);
        collapsedHeight = sizeHint().height() - contentArea->maximumHeight();

        updateHeights();
    }

    void ExamWidget::setTitle(QString title)
    {
        btnToggle->setText(std::move(title));
    }

    void ExamWidget::updateHeights()
    {
        int contentHeight = contentArea->layout()->sizeHint().height() + 40;

        for (int i = 0; i < toggleAnimation->animationCount() - 1; ++i) {
            QPropertyAnimation* SectionAnimation = static_cast<QPropertyAnimation*>(toggleAnimation->animationAt(i));
            SectionAnimation->setDuration(animationDuration);
            SectionAnimation->setStartValue(collapsedHeight);
            SectionAnimation->setEndValue(collapsedHeight + contentHeight);
        }

        QPropertyAnimation* contentAnimation = static_cast<QPropertyAnimation*>(toggleAnimation->animationAt(toggleAnimation->animationCount() - 1));
        contentAnimation->setDuration(animationDuration);
        contentAnimation->setStartValue(0);
        contentAnimation->setEndValue(contentHeight);

        toggleAnimation->setDirection(isExpanded ? QAbstractAnimation::Forward : QAbstractAnimation::Backward);
        toggleAnimation->start();
    }

    void ExamWidget::btnFolderOnClick()
    {
        QDesktopServices::openUrl(QUrl::fromLocalFile(exam.path()));
    }
}
}
