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


#ifndef SIDEBAR_H
#define SIDEBAR_H

#include <QtGui/QWidget>
#include <QtGui/QAbstractItemView>
#include <QtGui/QSortFilterProxyModel>
#include <Nepomuk/Resource>

class NotesView;
class NotesModel;

class Sidebar : public QWidget
{
    Q_OBJECT
public:
    explicit Sidebar(QWidget* parent = 0, Qt::WindowFlags f = 0);
    virtual ~Sidebar();

public slots:
    void noteSaved(const Nepomuk::Resource &note);

signals:
    void noteSelected(const Nepomuk::Resource &note);

private slots:
    void slotNoteSelected(const QModelIndex &index);

private:
    NotesView *m_notesView;
    NotesModel *m_notesModel;

    QSortFilterProxyModel *m_sortingModel;
};

#endif // SIDEBAR_H