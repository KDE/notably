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


#include "tageditor.h"

#include <QtGui/QPaintEvent>
#include <QtGui/QTextDocument>
#include <QtGui/QAbstractTextDocumentLayout>

#include <KDebug>

TagEditor::TagEditor(QWidget* parent)
    : KTextEdit(parent)
{
    // Don't keep scroll bars
    setVerticalScrollBarPolicy( Qt::ScrollBarAlwaysOff );
    setHorizontalScrollBarPolicy( Qt::ScrollBarAlwaysOff );

    setContextMenuPolicy( Qt::NoContextMenu );

    connect( document()->documentLayout(), SIGNAL(documentSizeChanged(QSizeF)),
             this, SLOT(slotDocumentSizeChanged()) );

    slotDocumentSizeChanged();
}

TagEditor::~TagEditor()
{

}

void TagEditor::addTag(const Nepomuk::Tag& tag)
{
    m_tagList << tag;
    emit tagsChanged( m_tagList );
}

void TagEditor::setTags(const QSet<Nepomuk::Tag>& tags)
{
    m_tagList = tags;
    emit tagsChanged( m_tagList );
}

QSet< Nepomuk::Tag > TagEditor::tags() const
{
    return m_tagList;
}


void TagEditor::paintEvent(QPaintEvent* event)
{
    KTextEdit::paintEvent(event);
}

void TagEditor::slotDocumentSizeChanged()
{
    setGeometry( x(), y(), width(), document()->documentLayout()->documentSize().height() );

    //FIXME: There must be better way!
    setMaximumHeight( document()->documentLayout()->documentSize().height() );
}

void TagEditor::keyPressEvent(QKeyEvent* event)
{
    if( event->key() == Qt::Key_Return || event->key() == Qt::Key_Enter ) {
        event->ignore();
        return;
    }

    KTextEdit::keyPressEvent(event);
}

QSize TagEditor::sizeHint() const
{
    return QSize( document()->idealWidth(), document()->documentLayout()->documentSize().height() );
}


