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


#ifndef TAGWIDGET_H
#define TAGWIDGET_H

#include <QtGui/QStringListModel>

#include <Nepomuk/Tag>

class TagEditor;
class TagView;

class TagWidget : public QWidget
{
    Q_OBJECT
public:
    explicit TagWidget(QWidget* parent = 0, Qt::WindowFlags f = 0);

    QList<Nepomuk::Tag> tags();
    void setTags(const QList<Nepomuk::Tag>& tags);
    void addTags(const QList<Nepomuk::Tag>& tags);

signals:
    void tagSelected(const Nepomuk::Tag &tag);

private slots:
    void slotAddTags();

private:
    TagEditor *m_tagEditor;
    TagView *m_tagView;
};

#endif // TAGWIDGET_H
