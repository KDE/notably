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


#ifndef PERSONGRID_H
#define PERSONGRID_H

#include <QtGui/QListView>
#include <QtCore/QEvent>
#include <QtCore/QTimer>

#include "persontooltip.h"
#include "personmodel.h"

class PersonGrid : public QListView
{
    Q_OBJECT
public:
    explicit PersonGrid(QWidget* parent = 0);

    void setPeople(const QList<Nepomuk2::Resource> &resources);
    QList<Nepomuk2::Resource> people() const;

signals:
    void personSelected(const Nepomuk2::Resource& person);

protected:
    virtual void leaveEvent(QEvent* event);

private slots:
    void startTimer(const QModelIndex& index);
    void showToolTip();
    void hideToolTip();

    void slotSelected(const QModelIndex& index);
private:
    PersonModel* m_model;
    PersonToolTip * m_tooltip;
    QTimer m_tooltipTimer;
    QModelIndex m_toolTipIndex;
};

#endif // PERSONGRID_H
