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


#include "annotationgrouptextobject.h"
#include "annotation/textannotationgroup.h"

#include <QtGui/QFontMetrics>
#include <QtGui/QPainter>
#include <QTextBlock>

#include <KDebug>

void AnnotationGroupTextObject::drawObject(QPainter* painter, const QRectF& rect, QTextDocument* doc,
                                  int posInDocument, const QTextFormat& format)
{
    QFont font = doc->findBlock( posInDocument ).charFormat().font();
    QString name = format.property( AnnotationText ).toString();
    TextAnnotationGroup* tag = format.property( AnnotationData ).value<TextAnnotationGroup*>();

    QRectF rec( rect );
    //vHanda: Just looks better this
    rec.moveTop( rec.top() + 2 );

    painter->save();
    painter->setFont( font );
    //FIXME: Depend on the color scheme
    QColor color;
    switch( tag->state() ) {
        case TextAnnotationGroup::Accepted:
            color = Qt::green;
            break;

        case TextAnnotationGroup::Listed:
            color = Qt::red;
            break;

        case TextAnnotationGroup::Rejected:
            color = Qt::blue;
            break;
    }
    color.setAlphaF( 0.2 );

    painter->fillRect( rec, color );
    painter->drawText( rec, name );
    painter->restore();
}

QSizeF AnnotationGroupTextObject::intrinsicSize(QTextDocument* doc, int posInDocument,
                                       const QTextFormat& format)
{
    QFont font = doc->findBlock( posInDocument ).charFormat().font();
    QFontMetrics fm( font );

    QString name = format.property( AnnotationText ).toString();
    QRect rect = fm.boundingRect( name );

    return rect.size();
}

