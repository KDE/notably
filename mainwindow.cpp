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
#include "noteedit.h"

#include <QtCore/QPropertyAnimation>

#include <QtGui/QApplication>
#include <QtGui/QDesktopWidget>
#include <QtGui/QBoxLayout>

#include <KTextEdit>
#include <KWindowSystem>
#include <KLocale>

MainWindow::MainWindow(QWidget* parent, Qt::WindowFlags f)
    : KMainWindow(parent, f)
{
    QWidget *widget = new QWidget( this );
    QVBoxLayout *mainLayout = new QVBoxLayout( widget );

    m_newNoteButton = new KPushButton( i18n("New Note") );
    QHBoxLayout *buttonLayout = new QHBoxLayout();
    buttonLayout->addWidget( m_newNoteButton );

    m_noteEditor = new NoteEdit( this );

    mainLayout->addLayout( buttonLayout );
    mainLayout->addWidget( m_noteEditor );

    setCentralWidget( widget );

    //setWindowFlags( Qt::FramelessWindowHint  );
    //KWindowSystem::setState(winId(), NET::Sticky | NET::SkipTaskbar | NET::SkipPager | NET::KeepAbove );

    QRect scr = QApplication::desktop()->screenGeometry();
    move( scr.center() - rect().center() );

    //QPropertyAnimation *animation = new QPropertyAnimation( this, "geometry");
    //animation->setDuration(10000);
    //animation->setStartValue(QRect(0, 0, 100, 30));
    //animation->setEndValue(QRect(250, 250, 100, 30));

    //animation->start();

    connect( m_newNoteButton, SIGNAL(clicked(bool)), this, SLOT(slotNewNote()) );
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

void MainWindow::slotNewNote()
{
    m_noteEditor->save();
    m_noteEditor->reset();
    m_noteEditor->setFocus();
}

