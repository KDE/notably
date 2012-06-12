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


#include "tag.h"

#include <QtGui/QPainter>
#include <QtGui/QPaintEvent>
#include <QStyleOptionViewItemV4>

#include <KStyle>
#include <KLocale>
#include <KApplication>
#include <KGlobalSettings>
#include <KIcon>

Tag::Tag(QWidget* parent, Qt::WindowFlags f): QWidget(parent, f)
{

}

void Tag::setTag(const Nepomuk2::Tag& tag)
{
    m_tagResource = tag;
}

Nepomuk2::Tag Tag::tag() const
{
    return m_tagResource;
}

QSize Tag::sizeHint() const
{
    QFontMetrics fm(font());
    int x = 0;
    int y = 0;

    QString tagLabel = m_tagResource.genericLabel();

    int m_margin = 3;
    x += m_margin + fm.width( tagLabel ) + m_margin; // Text
    x += fm.height() + m_margin; // Icon
    y += fm.height() + m_margin*2;

    return QSize( x, y );
}

void Tag::mousePressEvent(QMouseEvent* event)
{
    QPoint pos = event->pos();
    QRect r = rect();

    int margin = 3;
    QString tagLabel = m_tagResource.genericLabel();
    r.setX( r.x() + margin + QFontMetrics(font()).width(tagLabel) + margin );
    if( r.contains(pos) ) {
        emit tagDeleted(m_tagResource);
    }
    else {
        emit tagSelected(m_tagResource);
    }

    //QWidget::mousePressEvent(event);
}

void Tag::paintEvent(QPaintEvent* event)
{
    QStyleOptionViewItemV4 option;
    option.initFrom( this );

    QPainter painter( this );
    painter.save();
    QPen origPen = painter.pen();
    //origPen.setWidth( 2 );
    painter.setBrush( option.palette.alternateBase() );
    painter.setRenderHint( QPainter::Antialiasing );
    painter.setPen( origPen );
    painter.drawRoundedRect( option.rect, 6, 6 );
    painter.restore();

    QString tagLabel = m_tagResource.genericLabel();
    QRect rect( option.rect );
    int m_margin = 3;
    rect.adjust( m_margin, m_margin, -m_margin, -m_margin );

    //painter.drawText( rect, tagLabel );
    QStyle* style = KApplication::style();
    style->drawItemText(&painter, rect, 0, palette(), true, tagLabel );

    QPixmap m_buttonPixmap;
    if( m_buttonPixmap.isNull() ) {
        QFontMetrics fm(font());
        KIcon icon("edit-delete");
        m_buttonPixmap =  icon.pixmap( QSize(fm.height(), fm.height()) );
    }

    rect.setX( rect.x() + QFontMetrics(font()).width(tagLabel) + m_margin );
    style->drawItemPixmap( &painter, rect, 0, m_buttonPixmap );
}

