/*
   This file is part of the Nepomuk Notes project.
   Copyright (C) 2011  Vishesh Handa <handa.vish@gmail.com>

   This program is free software; you can redistribute it and/or
   modify it under the terms of the GNU General Public License as
   published by the Free Software Foundation; either version 2 of
   the License or (at your option) version 3 or any later version
   accepted by the membership of KDE e.V. (or its successor approved
   by the membership of KDE e.V.), which shall act as a proxy
   defined in Section 14 of version 3 of the license.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

#include "noteedit.h"
#include "person/personmodel.h"
#include "person/persondelegate.h"

#include <QtGui/QKeyEvent>
#include <QtGui/QListView>
#include <QtGui/QScrollBar>
#include <QtGui/QFontMetrics>

#include <Nepomuk/Variant>
#include <Nepomuk/ResourceManager>

#include <Nepomuk/Vocabulary/NIE>
#include <Nepomuk/Vocabulary/PIMO>

#include <KDebug>

using namespace Nepomuk::Vocabulary;

NoteEdit::NoteEdit(QWidget* parent)
    : KTextEdit(parent)
{
    reset();

    setCheckSpellingEnabled( true );

//     setAutoFormatting( QTextEdit::AutoBulletList );
    PersonModel* model = new PersonModel( this );

    QListView* view = new QListView( this );
    view->setModel( model );
    view->setItemDelegateForColumn( 0, new PersonDelegate(this) );

    m_completer = new QCompleter( model, this );
    m_completer->setWidget( this );
    m_completer->setCompletionRole( Qt::DisplayRole );
    m_completer->setPopup( view );
    m_completer->setCaseSensitivity( Qt::CaseInsensitive );

    connect( m_completer, SIGNAL(activated(QString)), this, SLOT(insertCompletion(QString)) );
}

NoteEdit::~NoteEdit()
{
}

void NoteEdit::setResource(const Nepomuk::Resource& note)
{
    if( note.isValid() ) {
        reset();

        m_noteResource = note;
        setPlainText( m_noteResource.property( NIE::plainTextContent() ).toString() );
        moveCursor( QTextCursor::End );

        document()->setModified( false );
    }
}

Nepomuk::Resource NoteEdit::resource() const
{
    return m_noteResource;
}


void NoteEdit::save()
{
    kDebug() << "Saving : " << m_noteResource.resourceUri();
    kDebug() << toPlainText();
    m_noteResource.setProperty( NIE::plainTextContent(), toPlainText() );
    m_noteResource.setProperty( NIE::htmlContent(), toHtml() );
}

void NoteEdit::reset()
{
    clear();
    // Create a new note
    m_noteResource = Nepomuk::Resource( QUrl(), PIMO::Note() );
}

void NoteEdit::keyPressEvent(QKeyEvent* event)
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
        //event->ignore();
        //return;
    }

    KTextEdit::keyPressEvent(event);

    QString text = wordUnderCursor();
    kDebug() << "Word under cursor: " << text;
    if( text.isEmpty() )
        return;

    m_completer->setCompletionPrefix( text );

    if( text.length() < 2 )
        return;

    // Get the cursor at the start of the tag
    // FIXME: Find a better way!
    QTextCursor origCursor = textCursor();
    QTextCursor tc = textCursor();
    tc.movePosition( QTextCursor::Left, QTextCursor::MoveAnchor, text.length() );
    setTextCursor( tc );
    QRect cr = cursorRect();
    setTextCursor( origCursor );

    cr.setWidth( m_completer->popup()->sizeHintForColumn(0)
                 + m_completer->popup()->verticalScrollBar()->sizeHint().width()*5 );
    m_completer->complete( cr );
}

QString NoteEdit::wordUnderCursor() const
{
    QTextCursor tc = textCursor();
    static QSet<QChar> delimiters;
    if( delimiters.isEmpty() ) {
        delimiters.insert( QChar::fromAscii(',') );
        delimiters.insert( QChar::fromAscii('!') );
        delimiters.insert( QChar::fromAscii('.') );
    }

    tc.anchor();
    while( 1 ) {
        // The '-1' is cause the TextCursor is always placed between characters
        int pos = tc.position() - 1;
        if( pos < 0 )
            break;

        QChar ch = document()->characterAt(pos);
        // FIXME: Maybe use other delimiters as well
        if( ch.isSpace() || delimiters.contains(ch) )
            break;

        tc.movePosition( QTextCursor::Left, QTextCursor::KeepAnchor );
    }
    return tc.selectedText().trimmed();
}

void NoteEdit::insertCompletion(const QString& string)
{
    QString compPrefix = m_completer->completionPrefix();

    QTextCursor tc = textCursor();
    tc.anchor();
    tc.movePosition( QTextCursor::Left, QTextCursor::KeepAnchor, compPrefix.length() );
    tc.removeSelectedText();
    tc.insertText( string );

    setTextCursor( tc );
}
