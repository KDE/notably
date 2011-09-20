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
    m_margin = 4;
}

void NoteItemDelegate::paint(QPainter* painter, const QStyleOptionViewItem& option, const QModelIndex& index) const
{
    QStyle* style = KApplication::style();
    style->drawPrimitive(QStyle::PE_PanelItemViewItem, &option, painter);

    QRect rect = option.rect;
    rect.adjust( m_margin, m_margin, -m_margin, -m_margin );

    Nepomuk::Resource res = index.data( Nepomuk::Utils::SimpleResourceModel::ResourceRole ).value<Nepomuk::Resource>();

    QString plainTextContent = res.property( NIE::plainTextContent() ).toString();
    QDateTime creationDate = res.property( NAO::created() ).toDateTime();

    //TODO: Find a way to convert this date into "4 hours ago" format
    QString dateString = creationDate.toLocalTime().toString();

    painter->save();
    QFont f = painter->font();
    f.setBold( true );
    painter->setFont( f );
    style->drawItemText( painter, rect, Qt::AlignLeft | Qt::AlignTop, option.palette, true, dateString );
    painter->restore();

    rect.setY( rect.y() + QFontMetrics(f).height()/* + m_margin */);

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
    // FIXME: There may be a case where some part of the text gets repeated before and after the ...
    if( l < plainTextContent.length() ) {
        text = plainTextContent.left( l );
        text += QLatin1String(" .... ");
        text += plainTextContent.right( charPerLine-10 );
    }
    else {
        text = plainTextContent;
    }

    textDocument.setPlainText( text.simplified() );

    painter->save();
    painter->translate( rect.topLeft() );
    textDocument.drawContents( painter );
    painter->restore();
}

QSize NoteItemDelegate::sizeHint(const QStyleOptionViewItem& option, const QModelIndex& index) const
{
    Q_UNUSED( index );
    Q_UNUSED( option );

    QFontMetrics fm( QTextDocument().defaultFont() );

    int x = 0;
    int y = 0;

    y += m_margin + fm.height() + m_margin + (fm.height() * 4) + m_margin;

    return QSize( x, y );
}


