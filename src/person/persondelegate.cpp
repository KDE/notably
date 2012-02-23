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


#include "persondelegate.h"
#include "personmodel.h"

#include <QtGui/QPainter>
#include <QtGui/QStyle>
#include <QtGui/QStyleOptionViewItemV4>

#include <KStyle>
#include <KIcon>
#include <KDebug>
#include <KGlobalSettings>

#include <QApplication>

PersonDelegate::PersonDelegate(QObject* parent): QStyledItemDelegate(parent)
{
    m_spacing = 3;
    m_pictureSize = 32;
}

void PersonDelegate::paint(QPainter* painter, const QStyleOptionViewItem& option, const QModelIndex& index) const
{
    QStyleOptionViewItemV4 optv4( option );

    painter->save();
    painter->setRenderHints( QPainter::Antialiasing | QPainter::HighQualityAntialiasing | QPainter::SmoothPixmapTransform );
    painter->setClipRect( optv4.rect );

    QStyle * style = QApplication::style();
    style->drawPrimitive(QStyle::PE_PanelItemViewItem, &option, painter);

    QRect iconRect = optv4.rect;
    iconRect.setSize( QSize(m_pictureSize, m_pictureSize) );
    iconRect.moveTo( QPoint(iconRect.x() + m_spacing, iconRect.y() + m_spacing) );

    QUrl pictureUrl = index.model()->data( index, PersonModel::PictureRole ).toUrl();
    QPixmap pixmap;
    pixmap.load( pictureUrl.toLocalFile() );

    //TODO: Cache the pixmaps
    if( !pixmap.isNull() ) {
        pixmap = pixmap.scaled( iconRect.size(), Qt::KeepAspectRatio, Qt::SmoothTransformation );
    }
    else {
        pixmap = KIcon("im-user").pixmap( iconRect.size() );
    }

    style->drawItemPixmap( painter, option.rect, Qt::AlignVCenter | Qt::AlignLeft, pixmap );

    QString name = index.model()->data( index, Qt::DisplayRole ).toString();

    QRect nameRect( optv4.rect );
    nameRect.moveTo( nameRect.x() + iconRect.width() + m_spacing, nameRect.y() );

    style->drawItemText( painter, nameRect, Qt::AlignVCenter, optv4.palette, true, name );

    painter->restore();
}

QSize PersonDelegate::sizeHint(const QStyleOptionViewItem& option, const QModelIndex& index) const
{
    const QAbstractItemModel *model = index.model();
    QUrl pictureUrl = model->data( index, PersonModel::PictureRole ).toUrl();
    QString name = model->data( index, Qt::DisplayRole ).toString();

    QFontMetrics fm(option.font);

    QSize size( m_pictureSize, m_pictureSize );
    size.setWidth( m_spacing + fm.width(name) );

    return size;
}
