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


#include "tagview.h"
#include "tagdelegate.h"

#include <QtGui/QMouseEvent>

TagView::TagView(QWidget* parent): QListView(parent)
{
    m_delegate = new TagDelegate( this );
    setItemDelegate( m_delegate );

    // For Grid
    setFlow( QListView::LeftToRight );
    setWrapping( true );
    setResizeMode( QListView::Adjust );

    setSpacing( 2 );
    setSelectionMode(QAbstractItemView::NoSelection);
    setMouseTracking( true );
    setFocusPolicy(Qt::NoFocus);
    setMinimumHeight( 0 );
    setMaximumHeight( 30 * 3 );

    // Remove the background and border
    setAttribute( Qt::WA_NoSystemBackground );
    setAttribute( Qt::WA_NoBackground );
    setAutoFillBackground( false );
    setStyleSheet("background-color: transparent;");
    setFrameShape(QFrame::NoFrame);
}

void TagView::mousePressEvent(QMouseEvent* event)
{
    QPoint pos = event->pos();
    QModelIndex index = indexAt(pos);
    QRect rect = visualRect(index);

    QRect deleteRect( rect );
    QString tagLabel = index.data().toString();
    int margin = m_delegate->margin();
    deleteRect.setX( deleteRect.x() + margin + QFontMetrics(font()).width(tagLabel) + margin );
    if( deleteRect.contains(pos) )
        emit tagDeleted(index);
    else
        emit tagClicked(index);

    QAbstractItemView::mousePressEvent(event);
}

