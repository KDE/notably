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

#include <QtGui/QKeyEvent>

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
    QTextCursor cur = textCursor();
    cur.movePosition( QTextCursor::Left, QTextCursor::KeepAnchor );
    QString text = cur.selectedText();
    if( text.length() == 1 ) {
        QChar lastChar = text.at( 0 );

        // Do not allow double spaces while typing
        if( event->key() == Qt::Key_Space && lastChar.isSpace() ) {
            event->accept();
            return;
        }
    }

    KTextEdit::keyPressEvent( event );
}
