/*
   This file is part of the Nepomuk Notes project.
   Copyright (C) 2011  Vishesh Handa <handa.vish@gmail.com>

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


#include "titlebar.h"
#include "mainwindow.h"

#include <QtGui/QPainter>
#include <QtGui/QLabel>
#include <QtGui/QHBoxLayout>

#include <KLocale>
#include <KGlobalSettings>

TitleBar::TitleBar(MainWindow* window)
    : QWidget(window),
      m_mainWindow(window)
{
    QLabel *titleLabel = new QLabel( this );
    titleLabel->setText( i18n("Nepomuk Notes") );

    QFont labelFont = titleLabel->font();
    labelFont.setBold( true );
    titleLabel->setFont( labelFont );

    m_quitButton = new QToolButton( this );
    m_quitButton->setAutoRaise( true );
    m_quitButton->setFocusPolicy( Qt::NoFocus );
    m_quitButton->setIcon(KIcon("application-exit"));
    m_quitButton->setToolTip( i18nc("@info:tooltip Quits the application", "Quit") );
    m_quitButton->setWhatsThis( i18nc("@info:whatsthis", "Quits the application") );
    connect( m_quitButton, SIGNAL(clicked()), m_mainWindow, SLOT(close()) );

    m_menuButton = new QToolButton( this );
    m_menuButton->setAutoRaise( true );
    m_menuButton->setMenu( m_mainWindow->menu() );
    m_menuButton->setPopupMode( QToolButton::InstantPopup );
    m_menuButton->setFocusPolicy( Qt::NoFocus );
    m_menuButton->setIcon(KIcon("configure"));
    m_menuButton->setToolTip( i18nc("@info:tooltip", "Open Menu") );
    m_menuButton->setWhatsThis( i18nc("@info:whatsthis", "Opens the main menu.") );

    QHBoxLayout *buttonLayout = new QHBoxLayout;
    buttonLayout->addWidget( m_menuButton );
    buttonLayout->addWidget( m_quitButton );

    QHBoxLayout *layout = new QHBoxLayout( this );
    // The left margin is kept so that there is some space before the title
    layout->setContentsMargins( layout->margin(), 0, 0, 0 );
    layout->setSpacing( 0 );
    layout->addWidget( titleLabel, 100, Qt::AlignLeft );
    layout->addLayout( buttonLayout );

    // No background
    setAttribute( Qt::WA_OpaquePaintEvent );
}

TitleBar::~TitleBar()
{

}

void TitleBar::paintEvent(QPaintEvent* event)
{
    QWidget::paintEvent(event);
}

void TitleBar::resizeEvent(QResizeEvent* event)
{
    QWidget::resizeEvent(event);
}

