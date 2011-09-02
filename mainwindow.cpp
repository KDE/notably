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
#include "titlebar.h"
#include "config/windowsettings.h"

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
    m_titleBar = new TitleBar( this );

    setupActions();
    setupMenus();
    setupGUI();

    applyWindowGeometry();
}

MainWindow::~MainWindow()
{
    Settings::self()->writeConfig();
}

void MainWindow::setupGUI()
{
    QWidget *widget = new QWidget();
    setCentralWidget( widget );

    m_noteWidget = new NoteWidget( this );

    m_mainLayout = new QVBoxLayout(widget);
    m_mainLayout->setSpacing( 0 );
    m_mainLayout->setMargin( 0 );

    m_mainLayout->addWidget( m_titleBar );
    m_mainLayout->addWidget( m_noteWidget );

    // Window flags to make it look pretier
    setWindowFlags( Qt::FramelessWindowHint );
    KWindowSystem::setState(winId(), NET::Sticky | NET::SkipTaskbar | NET::SkipPager | NET::KeepAbove );

    // Blur background
    setAttribute(Qt::WA_TranslucentBackground);

#ifdef Q_WS_X11
    Atom net_wm_blur_region = XInternAtom(QX11Info::display(), "_KDE_NET_WM_BLUR_BEHIND_REGION", False);

    XChangeProperty(QX11Info::display(), winId(), net_wm_blur_region, XA_CARDINAL,
                    32, PropModeReplace, 0, 0);
#endif
}

void MainWindow::toggleWindowState()
{
    if( isVisible() )
        hide();
    else {
        show();
        KWindowSystem::setState(winId(), NET::Sticky | NET::SkipTaskbar | NET::SkipPager | NET::KeepAbove );
    }
//     bool visible = isVisible();
//     // Visible but not active
//     if(visible && !isActiveWindow()) {
//         KWindowSystem::activateWindow(winId());
//         KWindowSystem::forceActiveWindow(winId());
//     }
//     else {
//         // Call the showing/hiding animation based on the visibility
//     }
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

//    m_menu->addAction(actionCollection()->action("view-full-screen"));
//     m_menu->addAction(actionCollection()->action("keep-open"));
//
//     m_screenMenu = new KMenu(this);
//     connect(m_screenMenu, SIGNAL(triggered(QAction*)), this, SLOT(setScreen(QAction*)));
//     m_screenMenu->setTitle(i18nc("@title:menu", "Screen"));
//     m_menu->addMenu(m_screenMenu);
//
    m_windowWidthMenu = new KMenu(this);
    connect(m_windowWidthMenu, SIGNAL(triggered(QAction*)), this, SLOT(setWindowWidth(QAction*)));
    m_windowWidthMenu->setTitle(i18nc("@title:menu", "Width"));
    m_menu->addMenu(m_windowWidthMenu);

    m_windowHeightMenu = new KMenu(this);
    connect(m_windowHeightMenu, SIGNAL(triggered(QAction*)), this, SLOT(setWindowHeight(QAction*)));
    m_windowHeightMenu->setTitle(i18nc("@title:menu", "Height"));
    m_menu->addMenu(m_windowHeightMenu);

    updateWindowSizeMenus();

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

void MainWindow::configureApp()
{
    if (KConfigDialog::showDialog("settings")) return;

    KConfigDialog* settingsDialog = new KConfigDialog(this, "settings", Settings::self());
    settingsDialog->setFaceType(KPageDialog::List);
    connect(settingsDialog, SIGNAL(settingsChanged(QString)), this, SLOT(applySettings()));
    connect(settingsDialog, SIGNAL(hidden()), this, SLOT(activate()));
    connect(settingsDialog, SIGNAL(cancelClicked()), this, SLOT(applyWindowGeometry()));

    WindowSettings* windowSettings = new WindowSettings(settingsDialog);
    settingsDialog->addPage(windowSettings, i18nc("@title Preferences page name", "Window"), "Nepomuk Notes");
    connect(windowSettings, SIGNAL(updateWindowGeometry(int,int)),
            this, SLOT(setWindowGeometry(int,int)));

    //QWidget* behaviorSettings = new QWidget(settingsDialog);
    //Ui::BehaviorSettings behaviorSettingsUi;
    //behaviorSettingsUi.setupUi(behaviorSettings);
    //settingsDialog->addPage(behaviorSettings, i18nc("@title Preferences page name", "Behavior"),
    //                        "preferences-other");

    settingsDialog->show();
}

void MainWindow::applySettings()
{
    applyWindowGeometry();
    updateWindowSizeMenus();
}

void MainWindow::activate()
{
    KWindowSystem::activateWindow(winId());
}

void MainWindow::updateWindowWidthMenu()
{
    QAction* action = 0;

    if(m_windowWidthMenu->isEmpty()) {
        for(int i = 10; i <= 100; i += 10) {
            action = m_windowWidthMenu->addAction(QString::number(i) + '%');
            action->setCheckable(true);
            action->setData(i);
            action->setChecked(i == Settings::width());
        }
    } else {
        QListIterator<QAction*> i(m_windowWidthMenu->actions());

        while (i.hasNext()) {
            action = i.next();
            action->setChecked(action->data().toInt() == Settings::width());
        }
    }
}

void MainWindow::updateWindowHeightMenu()
{
    QAction* action = 0;

    if(m_windowWidthMenu->isEmpty()) {
        for(int i = 10; i <= 100; i += 10) {
            action = m_windowHeightMenu->addAction(QString::number(i) + '%');
            action->setCheckable(true);
            action->setData(i);
            action->setChecked(i == Settings::height());
        }
    } else {
        QListIterator<QAction*> i(m_windowHeightMenu->actions());

        while (i.hasNext()) {
            action = i.next();
            action->setChecked(action->data().toInt() == Settings::height());
        }
    }
}

void MainWindow::updateWindowSizeMenus()
{
    updateWindowHeightMenu();
    updateWindowWidthMenu();
}

void MainWindow::setWindowHeight(QAction* action)
{
    setWindowHeight(action->data().toInt());
}

void MainWindow::setWindowWidth(QAction* action)
{
    setWindowWidth(action->data().toInt());
}

void MainWindow::setWindowHeight(int height)
{
    Settings::setHeight(height);

    applyWindowGeometry();
    updateWindowHeightMenu();
}

void MainWindow::setWindowWidth(int width)
{
    Settings::setWidth(width);

    applyWindowGeometry();
    updateWindowWidthMenu();
}

void MainWindow::setWindowGeometry(int width, int height)
{
    setWindowWidth( width );
    setWindowHeight( height );
}


void MainWindow::applyWindowGeometry()
{
    //TODO: Find a better way of setting the width and height
    const QRect screen = QApplication::desktop()->screenGeometry();

    if( Settings::blurBackground() ) {
        showFullScreen();

        int w = screen.width() * (100-Settings::width())/100.0 * 0.5;
        int h = screen.height() * (100-Settings::height())/100.0 * 0.5;
        m_mainLayout->setContentsMargins( w, h, w, h );
    }
    else {
        QRect newGeometry;
        newGeometry.setWidth( screen.width() * Settings::width()/100.0 );
        newGeometry.setHeight( screen.height() * Settings::height()/100.0 );

        setGeometry( newGeometry );

        // Move to the center of the screen
        // TODO:: Make configurable
        move( screen.center().x() - (rect().width() * Settings::horziontalPosition()/100.0),
            screen.center().y() - (rect().height() * Settings::verticalPosition()/100.0) );
    }
}
