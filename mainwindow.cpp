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
#include <KDebug>
#include <KShortcut>
#include <KAction>

#ifdef Q_WS_X11
#include <QX11Info>
#include <X11/Xlib.h>
#include <X11/Xatom.h>
#endif

MainWindow::MainWindow(QWidget* parent, Qt::WindowFlags f)
    : KMainWindow(parent, f)
{
    QWidget *widget = new QWidget();
    QVBoxLayout *mainLayout = new QVBoxLayout(widget);

    m_newNoteButton = new KPushButton( i18n("New Note") );
    m_saveNoteButton = new KPushButton( i18n("Save Note") );

    QHBoxLayout *buttonLayout = new QHBoxLayout();
    buttonLayout->addWidget( m_newNoteButton );
    buttonLayout->addWidget( m_saveNoteButton );

    m_noteEditor = new NoteEdit( this );

    mainLayout->addLayout( buttonLayout );
    mainLayout->addWidget( m_noteEditor );

    setCentralWidget( widget );
    //showFullScreen();

    // Window flags to make it look pretier
    setWindowFlags( Qt::FramelessWindowHint );
    KWindowSystem::setState(winId(), NET::Sticky | NET::SkipTaskbar | NET::SkipPager | NET::KeepAbove );

    QRect screen = QApplication::desktop()->screenGeometry();

    // TODO: Make configurable
    QRect newGeometry;
    newGeometry.setWidth( screen.width() * 0.35 );
    newGeometry.setHeight( screen.height() * 0.55 );
    setGeometry( newGeometry );

    // Move to the center of the screen
    // TODO:: Make configurable
    move( screen.center().x() - rect().width()/2, screen.center().y() - rect().height()/2 );

    //QPropertyAnimation *animation = new QPropertyAnimation( this, "geometry");
    //animation->setDuration(10000);
    //animation->setStartValue(QRect(0, 0, 100, 30));
    //animation->setEndValue(QRect(250, 250, 100, 30));

    //animation->start();

    // Buttons
    connect( m_newNoteButton, SIGNAL(clicked(bool)), this, SLOT(slotNewNote()) );
    connect( m_saveNoteButton, SIGNAL(clicked(bool)), this, SLOT(slotSaveNote()) );

    // Add a shortcut
    // TODO: Again, make configurable
    KAction * toggleAction = new KAction( i18n("Toggle Window"), this );
    toggleAction->setObjectName( QLatin1String("toggle-window") );
    toggleAction->setGlobalShortcut( KShortcut( QKeySequence( Qt::ALT + Qt::Key_K ) ) );
    connect( toggleAction, SIGNAL(triggered(bool)), this, SLOT(toggleWindowState()) );

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



MainWindow::~MainWindow()
{
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
    m_noteEditor->save();
    m_noteEditor->reset();
    m_noteEditor->setFocus();
}

void MainWindow::slotSaveNote()
{
    m_noteEditor->save();
}
