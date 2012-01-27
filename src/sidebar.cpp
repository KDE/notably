/*
    <one line to give the library's name and an idea of what it does.>
    Copyright (C) 2011-12  Vishesh Handa <handa.vish@gmail.com>

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


#include "sidebar.h"
#include "notebrowser.h"
#include "mainmenu.h"
#include "noteinformation.h"

#include <QtCore/QCoreApplication>

#include <QtGui/QSortFilterProxyModel>
#include <QtGui/QBoxLayout>
#include <QtGui/QPushButton>
#include <QtGui/QStackedLayout>
#include <QtGui/QLabel>

#include <Soprano/Vocabulary/NAO>
#include <KDebug>
#include <KIcon>

using namespace Soprano::Vocabulary;

Sidebar::Sidebar(QWidget* parent, Qt::WindowFlags f)
    : QWidget(parent, f)
{
    QWidget *navigationWidget = new QWidget();
    QHBoxLayout *hLayout = new QHBoxLayout( navigationWidget );
    hLayout->setMargin(0);
    hLayout->setSpacing(0);

    m_backButton = new QPushButton(KIcon("draw-arrow-back"), "");
    m_backButton->setFlat(true);
    connect( m_backButton, SIGNAL(clicked(bool)), this, SLOT(slotMoveBackward()) );

    m_forwardButton = new QPushButton(KIcon("draw-arrow-forward"), "");
    m_forwardButton->setFlat(true);
    connect( m_forwardButton, SIGNAL(clicked(bool)), this, SLOT(slotMoveForward()) );

    m_title = new QLabel(QLatin1String("Main Menu"));

    hLayout->addWidget( m_backButton, 0, Qt::AlignLeft );
    hLayout->addWidget( m_title, 100, Qt::AlignCenter );
    hLayout->addWidget( m_forwardButton, 0, Qt::AlignRight );

    m_noteBrowser = new NoteBrowser( this );
    connect( m_noteBrowser, SIGNAL(noteSelected(Nepomuk::Resource)),
             this, SIGNAL(noteSelected(Nepomuk::Resource)) );

    m_mainMenu = new MainMenu( this );
    connect( m_mainMenu, SIGNAL(newNote()), this, SIGNAL(newNote()) );
    connect( m_mainMenu, SIGNAL(browseNotes()), this, SLOT(slotBrowseNotes()) );

    m_noteInfo = new NoteInformation( this );

    m_stackedLayout = new QStackedLayout();
    m_stackedLayout->addWidget( m_mainMenu );
    m_stackedLayout->addWidget( m_noteBrowser );
    m_stackedLayout->addWidget( m_noteInfo );
    m_stackedLayout->setSpacing( 0 );
    m_stackedLayout->setMargin( 0 );

    QVBoxLayout * mainLayout = new QVBoxLayout( this );
    mainLayout->setSpacing(0);
    mainLayout->setMargin(0);
    mainLayout->addWidget( navigationWidget );
    mainLayout->addItem(m_stackedLayout);

    updateButtons();
}

Sidebar::~Sidebar()
{
}

void Sidebar::slotBrowseNotes()
{
    setTitle("Browse Notes");
    m_stackedLayout->setCurrentWidget( m_noteBrowser );
    updateButtons();
}

void Sidebar::setTitle(const QString& title)
{
    m_title->setText(title);
}

void Sidebar::slotMoveBackward()
{
    int index = m_stackedLayout->currentIndex();
    if( index > 0 ) {
        index--;
        m_stackedLayout->setCurrentIndex(index);

        updateButtons();
    }
}

void Sidebar::slotMoveForward()
{
    int index = m_stackedLayout->currentIndex();
    if( index < m_stackedLayout->count()-1 ) {
        index++;
        m_stackedLayout->setCurrentIndex(index);

        updateButtons();
    }
}

void Sidebar::updateButtons()
{
    int index = m_stackedLayout->currentIndex();
    if( index == 0 ) {
        m_backButton->setDisabled( true );
        m_forwardButton->setDisabled( false );
    }
    else if( index == m_stackedLayout->count()-1 ) {
        m_backButton->setDisabled( false );
        m_forwardButton->setDisabled( true );
    }
    else {
        m_backButton->setDisabled( false );
        m_forwardButton->setDisabled( false );
    }
}

void Sidebar::push(QWidget* widget)
{
    int index = m_stackedLayout->currentIndex();
    // Remove all the widgets after index
    for(int i=index+1; i<m_stackedLayout->count(); i++)
        m_stackedLayout->takeAt(i);
    m_stackedLayout->addWidget( widget );
}

void Sidebar::showInfo(const Nepomuk::Resource& note)
{
    m_noteInfo->setNote( note );
    m_stackedLayout->setCurrentWidget( m_noteInfo );
    updateButtons();
}
