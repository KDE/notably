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


#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QtGui/QBoxLayout>

#include <KMainWindow>
#include <KPushButton>
#include <KMenu>
#include <KHelpMenu>
#include <KActionCollection>

class NoteEdit;
class TitleBar;
class TagEditor;

class MainWindow : public KMainWindow
{
    Q_OBJECT
public:
    MainWindow(QWidget *parent=0, Qt::WindowFlags f=0);
    virtual ~MainWindow();

    KMenu *menu() const { return m_menu; }
    KActionCollection *actionCollection() { return m_actionCollection; }

private slots:
    void toggleWindowState();
    void slotNewNote();
    void slotSaveNote();

    void configureKeys();
    void configureNotifications();
    void configureApp();

    void setWindowWidth(QAction *action);
    void setWindowHeight(QAction *action);

    void setWindowWidth( int width );
    void setWindowHeight( int height );

    void setWindowGeometry( int width, int height );
    void applyWindowGeometry();

    void applySettings();

    void activate();
private:
    void setupActions();
    void setupMenus();
    void setupGUI();

    void updateWindowSizeMenus();
    void updateWindowHeightMenu();
    void updateWindowWidthMenu();

    NoteEdit *m_noteEditor;

    KMenu *m_menu;
    KMenu *m_windowHeightMenu;
    KMenu *m_windowWidthMenu;
    KHelpMenu *m_helpMenu;
    KActionCollection *m_actionCollection;

    TitleBar *m_titleBar;
    TagEditor *m_tagEditor;

    QBoxLayout *m_mainLayout;
};

#endif // MAINWINDOW_H
