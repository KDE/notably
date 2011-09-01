/*
   This file is part of the Nepomuk Notes project.
   Copyright (C) 2011  Vishesh Handa <handa.vish@gmail.com>

   This code has been adapted from Yakuake's code
   Copyright (C) 2008-2011 by Eike Hein <hein@kde.org>

   This program is free software; you can redistribute it and/or
   modify it under the terms of the GNU General Public License as
   published by the Free Software Foundation; either version 2 of
   the License or (at your option) version 3 or any later version
   accepted by the membership of KDE e.V. (or its successor approved
   by the membership of KDE e.V.), which shall act as a proxy
   defined in Section 14 of version 3 of the license.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/


#include "windowsettings.h"
#include "settings.h"

WindowSettings::WindowSettings(QWidget* parent, Qt::WindowFlags f): QWidget(parent, f)
{
    setupUi( this );

    connect(kcfg_Width, SIGNAL(valueChanged(int)), this, SLOT(updateWidthSlider(int)));
    connect(widthSlider, SIGNAL(valueChanged(int)), this, SLOT(updateWidthSpinBox(int)));

    connect(kcfg_Height, SIGNAL(valueChanged(int)), this, SLOT(updateHeightSlider(int)));
    connect(heightSlider, SIGNAL(valueChanged(int)), this, SLOT(updateHeightSpinBox(int)));
}

WindowSettings::~WindowSettings()
{
}

void WindowSettings::updateWidthSlider(int width)
{
    widthSlider->setValue(width/10);

    emit updateWindowGeometry(width, kcfg_Height->value() /*kcfg_Position->value()*/);
}

void WindowSettings::updateWidthSpinBox(int width)
{
    kcfg_Width->setValue(width*10);
}

void WindowSettings::updateHeightSlider(int height)
{
    heightSlider->setValue(height/10);

    emit updateWindowGeometry(kcfg_Width->value(), height /*kcfg_Position->value()*/);
}

void WindowSettings::updateHeightSpinBox(int height)
{
    kcfg_Height->setValue(height*10);
}
