/*
    <one line to give the library's name and an idea of what it does.>
    Copyright (C) 2011  Vishesh Handa <handa.vish@gmail.com>

    This library is free software; you can redistribute it and/or
    modify it under the terms of the GNU Lesser General Public
    License as published by the Free Software Foundation; either
    version 2.1 of the License, or (at your option) any later version.

    This library is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
    Lesser General Public License for more details.

    You should have received a copy of the GNU Lesser General Public
    License along with this library; if not, write to the Free Software
    Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA
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
