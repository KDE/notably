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


#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <KMainWindow>
#include <KPushButton>
#include <KMenu>
#include <KHelpMenu>
#include <KActionCollection>

class NoteEdit;
class TitleBar;

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

    void applyWindowGeometry();

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
};

#endif // MAINWINDOW_H
