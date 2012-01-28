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

TagDelegate::TagDelegate(QObject* parent): QStyledItemDelegate(parent)
{
    m_margin = 4;
}

void TagDelegate::paint(QPainter* painter, const QStyleOptionViewItem& option, const QModelIndex& index) const
{
    painter->save();
    QStyle* style = KApplication::style();
    style->drawPrimitive(QStyle::PE_PanelTipLabel, &option, painter);
    painter->drawRoundedRect( option.rect, 6, 6 );

    QString tagLabel = index.data().toString();
    QRect rect( option.rect );
    rect.adjust( m_margin, m_margin, -m_margin, -m_margin );

    style->drawItemText(painter, rect, 0, option.palette, true, tagLabel );

    //QStyledItemDelegate::paint(painter, option, index);
    painter->restore();
}

QSize TagDelegate::sizeHint(const QStyleOptionViewItem& option, const QModelIndex& index) const
{
    Q_UNUSED( option );

    QFontMetrics fm( option.font );
    int x = 0;
    int y = 0;

    QString tagLabel = index.data().toString();

    x += fm.width( tagLabel ) + m_margin*2;
    y += fm.height() + m_margin*2;

    return QSize( x, y );
}
