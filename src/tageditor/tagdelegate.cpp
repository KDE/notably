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


#include "tagdelegate.h"

#include <QtGui/QPainter>
#include <QtGui/QStyle>
#include <QtGui/QStyleOptionViewItemV4>

#include <KStyle>
#include <KLocale>
#include <KApplication>
#include <KGlobalSettings>
#include <KIcon>

TagDelegate::TagDelegate(QObject* parent): QStyledItemDelegate(parent)
{
    m_margin = 3;
}

void TagDelegate::paint(QPainter* painter, const QStyleOptionViewItem& option, const QModelIndex& index) const
{
    QStyle* style = KApplication::style();

    painter->save();
    QPen origPen = painter->pen();
    if( option.state & QStyle::State_MouseOver )
        painter->setPen( option.palette.highlight().color() );
    painter->setBrush( option.palette.alternateBase() );
    painter->setRenderHint( QPainter::Antialiasing );
    painter->drawRoundedRect( option.rect, 6, 6 );
    painter->setPen( origPen );

    QString tagLabel = index.data().toString();
    QRect rect( option.rect );
    rect.adjust( m_margin, m_margin, -m_margin, -m_margin );

    style->drawItemText(painter, rect, 0, option.palette, true, tagLabel );

    if( m_buttonPixmap.isNull() ) {
        QFontMetrics fm(option.font);
        KIcon icon("edit-delete");
        const_cast<TagDelegate*>(this)->m_buttonPixmap =  icon.pixmap( QSize(fm.height(), fm.height()) );
    }

    QStyleOption buttonOpt(option);
    rect.setX( rect.x() + QFontMetrics(option.font).width(tagLabel) + m_margin );
    buttonOpt.rect = rect;
    style->drawItemPixmap( painter, rect, 0, m_buttonPixmap );
    painter->restore();
}

QSize TagDelegate::sizeHint(const QStyleOptionViewItem& option, const QModelIndex& index) const
{
    Q_UNUSED( option );

    QFontMetrics fm( option.font );
    int x = 0;
    int y = 0;

    QString tagLabel = index.data().toString();

    x += m_margin + fm.width( tagLabel ) + m_margin; // Text
    x += fm.height() + m_margin; // Icon
    y += fm.height() + m_margin*2;

    return QSize( x, y );
}
