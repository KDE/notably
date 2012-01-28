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
    painter->save();
    QStyle* style = KApplication::style();
    QStyleOptionViewItem textOption(option);
    textOption.rect.setWidth( textOption.rect.width() - (16+m_margin) );

    style->drawPrimitive(QStyle::PE_PanelTipLabel/*QStyle::PE_PanelItemViewItem*/, &textOption, painter);
    painter->drawRoundedRect( option.rect, 6, 6 );

    QString tagLabel = index.data().toString();
    QRect rect( option.rect );
    rect.adjust( m_margin, m_margin, -m_margin, -m_margin );

    style->drawItemText(painter, rect, 0, option.palette, true, tagLabel );

    //QStyledItemDelegate::paint(painter, option, index);
    KIcon icon("edit-delete");
    rect.setX( rect.x() + QFontMetrics(option.font).width(tagLabel) + m_margin );

    QStyleOption buttonOpt(option);
    buttonOpt.rect.setX( buttonOpt.rect.x() + QFontMetrics(option.font).width(tagLabel) + m_margin*2 );
    style->drawPrimitive(QStyle::PE_PanelButtonTool, &buttonOpt, painter);
    style->drawItemPixmap( painter, rect, 0, icon.pixmap(QSize(16,16)) );
    painter->restore();
}

QSize TagDelegate::sizeHint(const QStyleOptionViewItem& option, const QModelIndex& index) const
{
    Q_UNUSED( option );

    QFontMetrics fm( option.font );
    int x = 0;
    int y = 0;

    QString tagLabel = index.data().toString();

    x += fm.width( tagLabel ) + m_margin*2; // Text
    x += m_margin + 16; // Icon
    y += fm.height() + m_margin*2;

    return QSize( x, y );
}

bool TagDelegate::editorEvent(QEvent* event, QAbstractItemModel* model,
                              const QStyleOptionViewItem& option, const QModelIndex& index)
{
    return QStyledItemDelegate::editorEvent(event, model, option, index);
}