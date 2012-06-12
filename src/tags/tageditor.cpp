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
#include <QtGui/QPainter>

#include <KDebug>

TagEditor::TagEditor(QWidget* parent)
    : KTextEdit(parent)
{
    // Don't keep scroll bars
    setVerticalScrollBarPolicy( Qt::ScrollBarAlwaysOff );
    setHorizontalScrollBarPolicy( Qt::ScrollBarAlwaysOff );

    setContextMenuPolicy( Qt::NoContextMenu );
    setPlaceholderText( QLatin1String("Tags (comma separated)") );

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

void TagEditor::addTag(const Nepomuk2::Tag& tag)
{
    if( !m_tagList.contains( tag ) ) {
        m_tagList << tag;

        QTextCursor tc = textCursor();
        tc.insertText( tag.genericLabel() + QLatin1String(", ") );
        tc.movePosition( QTextCursor::EndOfLine );
        setTextCursor( tc );

        emit tagsChanged( m_tagList );
    }
}

void TagEditor::setTags(const QList<Nepomuk2::Tag>& tags)
{
    if( m_tagList == tags )
        return;

    m_tagList = tags;
    clear();

    if( m_tagList.isEmpty() )
        return;

    // Update the string
    QStringList list;
    foreach( const Nepomuk2::Tag &tag, m_tagList )
        list << tag.genericLabel();

    setPlainText( list.join(QLatin1String(", ")) );

    QTextCursor tc = textCursor();
    tc.movePosition( QTextCursor::EndOfLine );
    tc.insertText( QLatin1String(", ") );
    setTextCursor( tc );

    emit tagsChanged( m_tagList );
}

QList<Nepomuk2::Tag> TagEditor::tags() const
{
    // Parse all the tags from text
    QString text = toPlainText();
    QStringList tagStringList = text.split( QChar::fromAscii(',') );

    QList<Nepomuk2::Tag> tagList;
    foreach( const QString &tag, tagStringList ) {
        QString trimmedTag = tag.trimmed();
        if( !trimmedTag.isEmpty() )
            tagList << trimmedTag;
    }

    return tagList;
}

#if 0
void TagEditor::paintEvent(QPaintEvent* event)
{
    KTextEdit::paintEvent(event);

    QPainter painter(viewport());

    QPalette pal = palette();

    QRect lineRect = viewport()->rect();
    Qt::Alignment va( Qt::AlignVCenter ); //FIXME

    QFontMetrics fm = fontMetrics();
    int minLB = qMax(0, -fm.minLeftBearing());

    if( document()->isEmpty() ) {
        if(!hasFocus() && !m_placeholderText.isEmpty()) {
            QColor col = pal.text().color();
            col.setAlpha(128);
            QPen oldpen = painter.pen();
            painter.setPen(col);
            lineRect.adjust(minLB, 0, 0, 0);
            QString elidedText = fm.elidedText(m_placeholderText, Qt::ElideRight, lineRect.width());
            painter.drawText(lineRect, va, elidedText);
            painter.setPen(oldpen);
            return;
        }
    }
}
#endif

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

    KTextEdit::keyPressEvent(event);

    QString text = tagUnderCursor();
    if( text.isEmpty() )
        return;

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
    QString compPrefix = m_completer->completionPrefix();

    QTextCursor tc = textCursor();
    tc.anchor();
    tc.movePosition( QTextCursor::Left, QTextCursor::KeepAnchor, compPrefix.length() );
    tc.removeSelectedText();

    setTextCursor( tc );

    // TODO: If the tag already exists, then add some kind of animation where that tag is
    // highlighted for a second or so
    addTag( Nepomuk2::Tag( completion ) );
}

QString TagEditor::placeholderText() const
{
    return m_placeholderText;
}

void TagEditor::setPlaceholderText(const QString& text)
{
    m_placeholderText = text;
}

void TagEditor::reset()
{
    clear();
    m_tagList.clear();
}
