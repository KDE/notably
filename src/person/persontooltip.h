/*
    <one line to give the library's name and an idea of what it does.>
    Copyright (C) 2011-12  Vishesh Handa <handa.vish@gmail.com>

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


#ifndef PERSONTOOLTIP_H
#define PERSONTOOLTIP_H

#include <QtGui/QWidget>
#include <QtGui/QLabel>
#include <QtGui/QTreeWidget>

#include <Nepomuk2/Resource>

class PersonToolTip : public QWidget
{
    Q_OBJECT
public:
    PersonToolTip(QWidget* parent = 0, Qt::WindowFlags f = 0);
    virtual ~PersonToolTip();

    void setPerson(const Nepomuk2::Resource& resource);
    Nepomuk2::Resource person() const;
protected:
    virtual void paintEvent(QPaintEvent* event );

private:
    QLabel *m_name;
    QLabel *m_preview;
    QTreeWidget *m_treeWidget;

    Nepomuk2::Resource m_person;
};

#endif // PERSONTOOLTIP_H
