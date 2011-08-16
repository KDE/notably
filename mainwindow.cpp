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


#include "mainwindow.h"

#include <QtCore/QPropertyAnimation>

#include <KTextEdit>
#include <KWindowSystem>

MainWindow::MainWindow(QWidget* parent, Qt::WindowFlags f)
    : KMainWindow(parent, f)
{
    KTextEdit *textEdit = new KTextEdit( this );
    setCentralWidget( textEdit );

    int x = maximumWidth()/2 - width()/2;
    int y = maximumHeight()/2 - height()/2;
    move( x, y );

    KWindowSystem::setState(winId(), NET::Sticky | NET::SkipTaskbar | NET::SkipPager | NET::KeepAbove );
    //KWindowSystem::setType(winId(), NET::Splash);

    //QPropertyAnimation *animation = new QPropertyAnimation( this, "geometry");
    //animation->setDuration(10000);
    //animation->setStartValue(QRect(0, 0, 100, 30));
    //animation->setEndValue(QRect(250, 250, 100, 30));

    //animation->start();
}

MainWindow::~MainWindow()
{
}

void MainWindow::toggleWindowState()
{
    bool visible = isVisible();
    // Visible but not active
    if(visible && !isActiveWindow()) {
        KWindowSystem::activateWindow(winId());
        KWindowSystem::forceActiveWindow(winId());
    }
    else {
        // Call the showing/hiding animation based on the visibility
    }
}

