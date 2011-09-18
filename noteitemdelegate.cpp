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


#include "noteitemdelegate.h"

#include <QtCore/QDateTime>

#include <QtGui/QPainter>
#include <QtGui/QStyle>
#include <QtGui/QTextDocument>
#include <QtGui/QStyleOptionViewItemV4>

#include <KStyle>
#include <KLocale>
#include <KApplication>
#include <KGlobalSettings>

#include <Nepomuk/Variant>
#include <Nepomuk/Utils/SimpleResourceModel>

#include <Nepomuk/Vocabulary/NIE>
#include <Soprano/Vocabulary/NAO>
#include <KDebug>

using namespace Soprano::Vocabulary;
using namespace Nepomuk::Vocabulary;

NoteItemDelegate::NoteItemDelegate(QObject* parent)
    : QStyledItemDelegate(parent)
{

}

void NoteItemDelegate::paint(QPainter* painter, const QStyleOptionViewItem& option, const QModelIndex& index) const
{
    QStyle* style = KApplication::style();
    style->drawPrimitive(QStyle::PE_PanelItemViewItem, &option, painter);

    QRect rect = option.rect;
    rect.setY( rect.y() + 5 );
    rect.setX( rect.x() + 5 );

    Nepomuk::Resource res = index.data( Nepomuk::Utils::SimpleResourceModel::ResourceRole ).value<Nepomuk::Resource>();

    QString plainTextContent = res.property( NIE::plainTextContent() ).toString();
    plainTextContent = plainTextContent.simplified();

    QDateTime creationDate = res.property( NAO::created() ).toDateTime();
    //kDebug() << creationDate;

    QString dateString = creationDate.toString();

    QPalette pal = option.palette;
    painter->save();
    QFont f = painter->font();
    f.setBold( true );
    painter->setFont( f );
    style->drawItemText( painter, rect, Qt::AlignLeft | Qt::AlignTop, pal, true, dateString, QPalette::Dark );
    painter->restore();
    rect.setY( rect.y() + 15 );

    //
    // Draw the excerpt
    //
    QTextDocument textDocument;
    textDocument.setTextWidth( rect.width() );

    QFont font = textDocument.defaultFont();
    font.setItalic( true );
    textDocument.setDefaultFont( font );

    QFontMetrics fm( font );
    int numLines = rect.height() / fm.height();
    int charPerLine = rect.width() / fm.averageCharWidth();

    int l = (numLines-2) * charPerLine; // one line less for ending, and one line for padding
    QString text;
    if( l < plainTextContent.length() ) {
        text = plainTextContent.left( l );
        text += QLatin1String(" .... ");
        text += plainTextContent.right( charPerLine-10 );
    }
    else {
        text = plainTextContent;
    }

    textDocument.setPlainText( text );

    painter->save();
    painter->translate( rect.topLeft() );
    textDocument.drawContents( painter );
    painter->restore();
}

QSize NoteItemDelegate::sizeHint(const QStyleOptionViewItem& option, const QModelIndex& index) const
{
    Q_UNUSED( index );

    QSize size = option.decorationSize;
    size += QSize( 40, 70 );

    return size;
}


