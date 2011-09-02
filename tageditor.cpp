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
#include "tagcompleter.h"

#include <QtGui/QPaintEvent>
#include <QtGui/QTextDocument>
#include <QtGui/QAbstractTextDocumentLayout>
#include <QtGui/QAbstractItemView>
#include <QtGui/QScrollBar>

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

    m_completer = new TagCompleter( this );
    m_completer->setWidget( this );
    m_completer->setCompletionMode( QCompleter::PopupCompletion );

    connect( m_completer, SIGNAL(activated(QString)), this, SLOT(insertCompletion(QString)) );
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
    if( m_completer->popup()->isVisible() ) {
        switch (event->key()) {
            case Qt::Key_Enter:
            case Qt::Key_Return:
            case Qt::Key_Escape:
            case Qt::Key_Tab:
            case Qt::Key_Backtab:
                event->ignore();
                return; // let the completer do default behavior
            default:
                break;
        }
    }
    else if( event->key() == Qt::Key_Return || event->key() == Qt::Key_Enter ) {
        event->ignore();
        return;
    }

    QTextEdit::keyPressEvent(event);

    QString text = tagUnderCursor();
    if( text.isEmpty() )
        return;

    kDebug() << "Setting prefix: " << text;
    m_completer->setCompletionPrefix( text );

    // Get the cursor at the start of the tag
    // FIXME: Find a better way!
    QTextCursor origCursor = textCursor();
    QTextCursor tc = textCursor();
    tc.movePosition( QTextCursor::Left, QTextCursor::MoveAnchor, text.length() );
    setTextCursor( tc );
    QRect cr = cursorRect();
    setTextCursor( origCursor );

    cr.setWidth( m_completer->popup()->sizeHintForColumn(0)
                 + m_completer->popup()->verticalScrollBar()->sizeHint().width() );
    m_completer->complete( cr );
}

QString TagEditor::tagUnderCursor() const
{
    QTextCursor tc = textCursor();

    tc.anchor();
    while( 1 ) {
        // The '-1' is cause the TextCursor is always placed between characters
        int pos = tc.position() - 1;
        if( pos < 0 || document()->characterAt(pos) == QChar::fromAscii(',') )
            break;
        tc.movePosition( QTextCursor::Left, QTextCursor::KeepAnchor );
    }
    return tc.selectedText().trimmed();
}

QSize TagEditor::sizeHint() const
{
    return QSize( document()->idealWidth(), document()->documentLayout()->documentSize().height() );
}

void TagEditor::insertCompletion(const QString &completion)
{
    QTextCursor tc = textCursor();
    int extra = completion.length() - m_completer->completionPrefix().length();
    tc.movePosition(QTextCursor::Left);
    tc.movePosition(QTextCursor::EndOfWord);
    tc.insertText( completion.right(extra) );
    tc.insertText( QLatin1String(", ") ); // Insert the ", " at the end of the tag
    setTextCursor(tc);

    addTag( Nepomuk::Tag( completion ) );
}

