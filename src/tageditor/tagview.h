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


#ifndef TAGVIEW_H
#define TAGVIEW_H

#include <QtGui/QListView>

#include <Nepomuk/Tag>

class TagView : public QListView
{
    Q_OBJECT
public:
    explicit TagView(QWidget* parent = 0);

signals:
    void tagClicked(const QModelIndex& index);
    void tagDeleted(const QModelIndex& index);

protected:
    void mousePressEvent(QMouseEvent* event);
};

#endif // TAGVIEW_H
