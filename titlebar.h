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


#ifndef TITLEBAR_H
#define TITLEBAR_H

#include <QtGui/QWidget>

class MainWindow;
class KPushButton;

class TitleBar : public QWidget
{
public:
    explicit TitleBar(MainWindow* window = 0);
    virtual ~TitleBar();

    virtual void paintEvent(QPaintEvent* event);
    virtual void resizeEvent(QResizeEvent* event);

private:
    KPushButton *m_quitButton;
    KPushButton *m_menuButton;

    MainWindow *m_mainWindow;
};

#endif // TITLEBAR_H
