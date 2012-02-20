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
#include <KDebug>
#include <KGlobalSettings>

#include <QDebug>
#include <QApplication>

PersonDelegate::PersonDelegate(QObject* parent): QStyledItemDelegate(parent)
{

}

void PersonDelegate::paint(QPainter* painter, const QStyleOptionViewItem& option, const QModelIndex& index) const
{
    QStyle * style = QApplication::style();
    style->drawPrimitive(QStyle::PE_PanelItemViewItem, &option, painter);

    QUrl pictureUrl = index.model()->data( index, PersonModel::PictureRole ).toUrl();
    QPixmap pic;
    pic.load( pictureUrl.toLocalFile() );

    QString name = index.model()->data( index, Qt::DisplayRole ).toString();

    pic = pic.scaled( option.rect.width(), option.rect.height(), Qt::KeepAspectRatio );
    style->drawItemPixmap( painter, option.rect, Qt::AlignCenter, pic );

    if( option.state & QStyle::State_Selected )
        painter->fillRect( option.rect, option.palette.highlight() );

    QRect rect = option.rect;
    rect.setLeft( rect.left() + pic.width() );
    style->drawItemText( painter, rect, Qt::AlignVCenter, option.palette, true, name );
    //painter->drawText( rect.x(), pos.y() - option.fontMetrics.height(), name );

    //if( ) {
        //FIXME: This is so hacky!
    //    pos += QPoint( 32 + decorationSize.width()/2 , 32 );
    //}
    //painter->drawText( rect.topLeft().x()+20, rect.topLeft().y(), QLatin1String("fire!") );
    //QStyledItemDelegate::paint(painter, option, index);
}

QSize PersonDelegate::sizeHint(const QStyleOptionViewItem& option, const QModelIndex& index) const
{
    const QAbstractItemModel *model = index.model();
    QPixmap pic = model->data( index, PersonModel::PictureRole ).value<QPixmap>();
    QString name = model->data( index, Qt::DisplayRole ).toString();

//     int w = 0;
//     w += 32;
//     w += option.fontMetrics.width( name );

    QSize size = option.decorationSize;
    size.setWidth( size.width() + 32 );
    size.setHeight( size.height() + 32 );

    //qDebug() << size;
    return size;
    //return pic.size() + option.decorationSize;
    //return QStyledItemDelegate::sizeHint(option, index);
}
