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

#include <QtCore/QEvent>
#include <QtGui/QApplication>
#include <QPainter>

#include <KDebug>
#include <KIcon>

class PersonGridDelegate: public QStyledItemDelegate {
public:
    explicit PersonGridDelegate(QObject* parent = 0);

    void paint(QPainter* painter, const QStyleOptionViewItem& option, const QModelIndex& index) const;
    QSize sizeHint(const QStyleOptionViewItem& option, const QModelIndex& index) const;
};

PersonGridDelegate::PersonGridDelegate(QObject* parent): QStyledItemDelegate(parent)
{
}

void PersonGridDelegate::paint(QPainter* painter, const QStyleOptionViewItem& option, const QModelIndex& index) const
{
    painter->setRenderHint( QPainter::HighQualityAntialiasing );
    painter->setRenderHint( QPainter::SmoothPixmapTransform );

    QStyle * style = QApplication::style();
    style->drawPrimitive(QStyle::PE_PanelItemViewItem, &option, painter);

    QUrl pictureUrl = index.model()->data( index, PersonModel::PictureRole ).toUrl();
    QPixmap pixmap;
    if( pictureUrl.isEmpty() ) {
        pixmap = KIcon("im-user").pixmap( QSize(32, 32) );
    }
    else {
        pixmap.load( pictureUrl.toLocalFile() );
        pixmap = pixmap.scaled( option.rect.width(), option.rect.height(), Qt::KeepAspectRatio );
    }

    style->drawItemPixmap( painter, option.rect, Qt::AlignCenter, pixmap );
}

QSize PersonGridDelegate::sizeHint(const QStyleOptionViewItem& option, const QModelIndex& index) const
{
    return QSize(32,32);
}


PersonGrid::PersonGrid(QWidget* parent): QListView(parent)
{
    setItemDelegate( new PersonGridDelegate(this) );

    setModel( new PersonModel(this) );
    setFlow( QListView::LeftToRight );
    setWrapping( true );
    setResizeMode( QListView::Adjust );
    setSpacing( 1 );

    m_tooltip = new PersonToolTip( Nepomuk::Resource(), this );
    m_tooltipTimer.setSingleShot( true );
    m_tooltipTimer.setInterval( 800 );
    connect( &m_tooltipTimer, SIGNAL(timeout()), this, SLOT(showToolTip()) );

    setMouseTracking( true );
    connect( this, SIGNAL(entered(QModelIndex)), this, SLOT(startTimer(QModelIndex)) );
    connect( this, SIGNAL(viewportEntered()), this, SLOT(hideToolTip()) );
}

void PersonGrid::showToolTip()
{
    QRect rect = visualRect(m_toolTipIndex);
    rect.setTopLeft( mapToGlobal(rect.topLeft()) );
    rect.setBottomRight( mapToGlobal(rect.bottomRight()) );

    m_tooltip->show();
    m_tooltip->move( rect.left() + rect.width()/2 - m_tooltip->sizeHint().width()/2, rect.bottom() );
}

void PersonGrid::hideToolTip()
{
    m_tooltip->hide();
    m_tooltipTimer.stop();
}

void PersonGrid::startTimer(const QModelIndex& index)
{
    hideToolTip();

    m_toolTipIndex = index;
    m_tooltipTimer.start();
}

void PersonGrid::leaveEvent(QEvent* event)
{
    hideToolTip();

    QWidget::leaveEvent(event);
}
