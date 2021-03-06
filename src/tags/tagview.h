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

#include "flowlayout.h"
#include "tag.h"
#include <QtGui/QListView>

#include <Nepomuk2/Tag>

class Tag;

class TagView : public QWidget
{
    Q_OBJECT
public:
    explicit TagView(QWidget* parent = 0);

    void addTags( const QList<Nepomuk2::Tag> &tags );
    void setTags( const QList<Nepomuk2::Tag> &tags );
    void clearTags();
    QList<Nepomuk2::Tag> tags();

signals:
    void tagClicked(const Nepomuk2::Tag& tag);

private slots:
    void slotTagDeleted(const Nepomuk2::Tag& tag);

private:
    FlowLayout* m_layout;
    QList<Tag*> m_tags;
};

#endif // TAGVIEW_H
