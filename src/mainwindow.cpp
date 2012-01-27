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


#include "mainwindow.h"
#include "notewidget.h"
#include "settings.h"
#include "sidebar.h"
#include "config/windowsettings.h"

#include <QtCore/QPropertyAnimation>
#include <QtGui/QApplication>
#include <QtGui/QDesktopWidget>
#include <QtGui/QBoxLayout>

#include <Nepomuk/ResourceManager>

#include <KTextEdit>
#include <KWindowSystem>
#include <KLocale>
#include <KDebug>
#include <KShortcut>
#include <KShortcutsDialog>
#include <KAction>
#include <KToolBar>
#include <KNotifyConfigWidget>
#include <KConfigDialog>

#ifdef Q_WS_X11
#include <QX11Info>
#include <X11/Xlib.h>
#include <X11/Xatom.h>
#endif

MainWindow::MainWindow(QWidget* parent, Qt::WindowFlags f)
    : KMainWindow(parent, f)
{
    m_menu = new KMenu(this);
    m_helpMenu = new KHelpMenu(this, KGlobal::mainComponent().aboutData());

    setupActions();
    setupMenus();
    setupGUI();

    setAutoSaveSettings();
}

MainWindow::~MainWindow()
{
    Settings::self()->writeConfig();
    saveGeometry();
}

void MainWindow::setupGUI()
{
    QWidget *outerWidget = new QWidget( this );
    setCentralWidget( outerWidget );

    m_noteWidget = new NoteWidget( this );
    m_sidebar = new Sidebar( this );

    m_mainLayout = new QHBoxLayout( outerWidget );
    m_mainLayout->addWidget( m_noteWidget );
    m_mainLayout->addWidget( m_sidebar );

    connect( m_sidebar, SIGNAL(noteSelected(Nepomuk::Resource)), m_noteWidget, SLOT(setNote(Nepomuk::Resource)) );
    connect( m_sidebar, SIGNAL(newNote()), m_noteWidget, SLOT(newNote()) );
    connect( m_noteWidget, SIGNAL(infoRequired(Nepomuk::Resource)), m_sidebar, SLOT(showInfo(Nepomuk::Resource)) );
}

void MainWindow::toggleWindowState()
{
    if( isVisible() ) {
        // Start the hide animation
        QPropertyAnimation *fadeAnimation = new QPropertyAnimation( this, "windowOpacity" );
        fadeAnimation->setDuration( Settings::duration() );
        fadeAnimation->setStartValue( 1.0f );
        fadeAnimation->setEndValue( 0.0f );

        connect( fadeAnimation, SIGNAL(finished()), this, SLOT(hide()) );
        connect( fadeAnimation, SIGNAL(finished()), fadeAnimation, SLOT(deleteLater()) );
        fadeAnimation->start();
    }
    else {
        show();

        // Start the show animation
        QPropertyAnimation *fadeAnimation = new QPropertyAnimation( this, "windowOpacity" );
        fadeAnimation->setDuration( Settings::duration() );
        fadeAnimation->setStartValue( 0.0f );
        fadeAnimation->setEndValue( 1.0f );

        connect( fadeAnimation, SIGNAL(finished()), fadeAnimation, SLOT(deleteLater()) );
        fadeAnimation->start();
    }
}

void MainWindow::slotNewNote()
{
    m_noteWidget->newNote();
}

void MainWindow::slotSaveNote()
{
    m_noteWidget->saveNote();
}

void MainWindow::setupActions()
{
    m_actionCollection = new KActionCollection( this );

    KAction *action = KStandardAction::quit(this, SLOT(close()), actionCollection());

    // About stuff
    action = KStandardAction::aboutApp( m_helpMenu, SLOT(aboutApplication()), actionCollection() );
    action = KStandardAction::reportBug( m_helpMenu, SLOT(reportBug()), actionCollection() );
    action = KStandardAction::aboutKDE( m_helpMenu, SLOT(aboutKDE()), actionCollection() );

    // Configuration
    action = KStandardAction::keyBindings(this, SLOT(configureKeys()), actionCollection());
    action = KStandardAction::configureNotifications(this, SLOT(configureNotifications()), actionCollection());
    action = KStandardAction::preferences(this, SLOT(configureApp()), actionCollection());

    //action = KStandardAction::whatsThis(this, SLOT(whatsThis()), actionCollection());

    // Add a shortcut
    action = actionCollection()->addAction("toggle-window");
    action->setObjectName( QLatin1String("toggle-window") );
    action->setText(i18nc("@action:", "Toggle Window"));
    action->setGlobalShortcut( KShortcut( QKeySequence( Qt::ALT + Qt::Key_K ) ) );
    connect( action, SIGNAL(triggered()), this, SLOT(toggleWindowState()) );

    // Note stuff
    action = KStandardAction::openNew( this, SLOT(slotNewNote()), actionCollection() );
    action = KStandardAction::save( this, SLOT(slotSaveNote()), actionCollection() );

    m_actionCollection->associateWidget(this);
    m_actionCollection->readSettings(); // vHanda: Why is this done?
}

void MainWindow::setupMenus()
{
    m_menu->addTitle(i18nc("@title:menu", "Help"));
    //m_menu->addAction(actionCollection()->action(KStandardAction::stdName(KStandardAction::WhatsThis)));
    m_menu->addAction(actionCollection()->action(KStandardAction::stdName(KStandardAction::ReportBug)));
    m_menu->addAction(actionCollection()->action(KStandardAction::stdName(KStandardAction::AboutApp)));
    m_menu->addAction(actionCollection()->action(KStandardAction::stdName(KStandardAction::AboutKDE)));

    m_menu->addTitle(i18nc("@title:menu", "Quick Options"));

    m_menu->addAction( actionCollection()->action(KStandardAction::name(KStandardAction::New)) );
    m_menu->addAction( actionCollection()->action(KStandardAction::name(KStandardAction::Save)) );

    m_menu->addTitle(i18nc("@title:menu", "Settings"));
    //m_menu->addAction(actionCollection()->action("manage-profiles"));
    m_menu->addAction(actionCollection()->action(KStandardAction::name(KStandardAction::KeyBindings)));
    m_menu->addAction(actionCollection()->action(KStandardAction::name(KStandardAction::ConfigureNotifications)));
    m_menu->addAction(actionCollection()->action(KStandardAction::name(KStandardAction::Preferences)));
}

void MainWindow::configureKeys()
{
    KShortcutsDialog::configure( actionCollection() );
}

void MainWindow::configureNotifications()
{
    KNotifyConfigWidget::configure(this);
}

void MainWindow::activate()
{
    KWindowSystem::activateWindow(winId());
}
