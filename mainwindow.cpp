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
#include "settings.h"
#include "tageditor.h"

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
#include <KNotifyConfigWidget>

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

    setupGUI();
    setupActions();
    setupMenus();
}

MainWindow::~MainWindow()
{
}

void MainWindow::setupGUI()
{
    QWidget *widget = new QWidget();
    QVBoxLayout *mainLayout = new QVBoxLayout(widget);

    m_newNoteButton = new KPushButton( i18n("New Note") );
    m_saveNoteButton = new KPushButton( i18n("Save Note") );

    QHBoxLayout *buttonLayout = new QHBoxLayout();
    buttonLayout->addWidget( m_newNoteButton );
    buttonLayout->addWidget( m_saveNoteButton );

    m_noteEditor = new NoteEdit( this );
    SemNotes::TagEditor *tagEditor = new SemNotes::TagEditor( this );

    mainLayout->addLayout( buttonLayout );
    mainLayout->addWidget( m_noteEditor );
    mainLayout->addWidget( tagEditor );

    setCentralWidget( widget );
    //showFullScreen();

    // Window flags to make it look pretier
    setWindowFlags( Qt::FramelessWindowHint );
    KWindowSystem::setState(winId(), NET::Sticky | NET::SkipTaskbar | NET::SkipPager | NET::KeepAbove );

    QRect screen = QApplication::desktop()->screenGeometry();

    // TODO: Make configurable
    QRect newGeometry;
    newGeometry.setWidth( screen.width() * Settings::width()/100.0 );
    newGeometry.setHeight( screen.height() * Settings::height()/100.0 );
    setGeometry( newGeometry );

    // Move to the center of the screen
    // TODO:: Make configurable
    move( screen.center().x() - (rect().width() * Settings::horziontalPosition()/100.0),
          screen.center().y() - (rect().height() * Settings::verticalPosition()/100.0) );

    // Buttons
    connect( m_newNoteButton, SIGNAL(clicked(bool)), this, SLOT(slotNewNote()) );
    connect( m_saveNoteButton, SIGNAL(clicked(bool)), this, SLOT(slotSaveNote()) );

    // Blur background
    setAttribute(Qt::WA_TranslucentBackground);
    setAttribute(Qt::WA_NoSystemBackground, false);
#ifdef Q_WS_X11
    Atom net_wm_blur_region = XInternAtom(QX11Info::display(), "_KDE_NET_WM_BLUR_BEHIND_REGION", False);

    QVector<QRect> rects;
    QRect re = screen;
    rects << re;

    QVector<unsigned long> data;
    foreach (const QRect &r, rects) {
        data << r.x() << r.y() << r.width() << r.height();
    }

    XChangeProperty(QX11Info::display(), winId(), net_wm_blur_region, XA_CARDINAL, 32, PropModeReplace,
                    reinterpret_cast<const unsigned char *>(data.constData()), data.size());
#endif
}

void MainWindow::toggleWindowState()
{
    kDebug();
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
    m_noteEditor->save();
    m_noteEditor->reset();
    m_noteEditor->setFocus();
}

void MainWindow::slotSaveNote()
{
    m_noteEditor->save();
}

void MainWindow::setupActions()
{
    m_actionCollection = new KActionCollection( this );

    KAction *action = KStandardAction::quit(this, SLOT(close()), actionCollection());
    action->setShortcut( Qt::CTRL + Qt::Key_Q );

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

//    m_menu->addTitle(i18nc("@title:menu", "Quick Options"));
//    m_menu->addAction(actionCollection()->action("view-full-screen"));
//     m_menu->addAction(actionCollection()->action("keep-open"));
//
//     m_screenMenu = new KMenu(this);
//     connect(m_screenMenu, SIGNAL(triggered(QAction*)), this, SLOT(setScreen(QAction*)));
//     m_screenMenu->setTitle(i18nc("@title:menu", "Screen"));
//     m_menu->addMenu(m_screenMenu);
//
//     m_windowWidthMenu = new KMenu(this);
//     connect(m_windowWidthMenu, SIGNAL(triggered(QAction*)), this, SLOT(setWindowWidth(QAction*)));
//     m_windowWidthMenu->setTitle(i18nc("@title:menu", "Width"));
//     m_menu->addMenu(m_windowWidthMenu);
//
//     m_windowHeightMenu = new KMenu(this);
//     connect(m_windowHeightMenu, SIGNAL(triggered(QAction*)), this, SLOT(setWindowHeight(QAction*)));
//     m_windowHeightMenu->setTitle(i18nc("@title:menu", "Height"));
//     m_menu->addMenu(m_windowHeightMenu);

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

}


