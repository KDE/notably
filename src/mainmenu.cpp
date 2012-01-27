/*
    <one line to give the library's name and an idea of what it does.>
    Copyright (C) 2012  Vishesh Handa <handa.vish@gmail.com>

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


#include "mainmenu.h"

#include <QtGui/QPushButton>
#include <QtGui/QVBoxLayout>

#include <KLocale>
#include <KIcon>

MainMenu::MainMenu(QWidget* parent, Qt::WindowFlags f): QWidget(parent, f)
{
    QPushButton *newNoteButton = new QPushButton(KIcon("document-new"), i18n("New Note"));
    QPushButton *browseButton = new QPushButton(KIcon("folder-grey"), i18n("Browse Notes"));
    QPushButton *settingsButton = new QPushButton(KIcon("configure"), i18n("Settings"));

    //FIXME: Find a better way!
    newNoteButton->setMaximumHeight( 5000 );
    browseButton->setMaximumHeight( 5000 );
    settingsButton->setMaximumHeight( 5000 );

    connect( newNoteButton, SIGNAL(clicked(bool)), this, SIGNAL(newNote()) );
    connect( browseButton, SIGNAL(clicked(bool)), this, SIGNAL(browseNotes()) );

    QVBoxLayout* layout = new QVBoxLayout( this );
    layout->addWidget( newNoteButton );
    layout->addWidget( browseButton );
    layout->addWidget( settingsButton );
    layout->setMargin( 0 );
    layout->setSpacing( 0 );
}
