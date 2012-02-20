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


#include "persongrid.h"
#include "persondelegate.h"
#include "personmodel.h"
#include <QEvent>

#include <KDebug>

Nepomuk::PersonGrid::PersonGrid(QWidget* parent): QListView(parent)
{
    setItemDelegate( new PersonDelegate(this) );

    setModel( new PersonModel(this) );
    setFlow( QListView::LeftToRight );
    setWrapping( true );
    setResizeMode( QListView::Adjust );
    setSpacing( 1 );

    m_tooltip = new Nepomuk::PersonToolTip( Nepomuk::Resource(), this );

    setMouseTracking( true );
    connect( this, SIGNAL(entered(QModelIndex)), this, SLOT(showToolTip(QModelIndex)) );
    connect( this, SIGNAL(viewportEntered()), this, SLOT(hideToolTip()) );
}

void Nepomuk::PersonGrid::showToolTip(const QModelIndex & index)
{
    QRect rect = visualRect(index);
    rect.setTopLeft( mapToGlobal(rect.topLeft()) );
    rect.setBottomRight( mapToGlobal(rect.bottomRight()) );

    m_tooltip->show();
    m_tooltip->move( rect.left() + rect.width()/2 - m_tooltip->sizeHint().width()/2, rect.bottom() );
}

void Nepomuk::PersonGrid::hideToolTip()
{
    m_tooltip->hide();
}

