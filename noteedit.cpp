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


#include "noteedit.h"

#include <QtGui/QKeyEvent>

#include <Soprano/Model>
#include <Soprano/QueryResultIterator>

#include <Nepomuk/Variant>
#include <Nepomuk/ResourceManager>

#include <Nepomuk/Vocabulary/PIMO>
#include <Nepomuk/Vocabulary/NIE>
#include <Soprano/Vocabulary/NAO>

#include <KDebug>

using namespace Nepomuk::Vocabulary;
using namespace Soprano::Vocabulary;

NoteEdit::NoteEdit(QWidget* parent)
    : KTextEdit(parent)
{
    // Show the last modified resource
    QString query = QString::fromLatin1("select ?r where { ?r a %1. ?r %2 ?dt . } "
                                        "ORDER BY desc(?dt) LIMIT 1")
                    .arg( Soprano::Node::resourceToN3( PIMO::Note() ),
                          Soprano::Node::resourceToN3( NAO::lastModified() ) );

    Soprano::Model *model = Nepomuk::ResourceManager::instance()->mainModel();
    Soprano::QueryResultIterator it = model->executeQuery( query, Soprano::Query::QueryLanguageSparql );
    kDebug() << "Executed " << query;
    if( it.next() ) {
        kDebug() << "Found " << it[0].uri();

        m_noteResource = Nepomuk::Resource( it[0].uri() );
        setPlainText( m_noteResource.property( NIE::plainTextContent() ).toString() );
        moveCursor( QTextCursor::End );
    }
    else {
        reset();
    }

    setCheckSpellingEnabled( true );

    // This is done so that when the user starts writing the note, the creation time is set
    connect( this, SIGNAL(textChanged()), this, SLOT(slotCreateNote()) );
}

NoteEdit::~NoteEdit()
{
    save();
}

void NoteEdit::save()
{
    const QString plainTextContent = m_noteResource.property( NIE::plainTextContent() ).toString();
    if( plainTextContent != toPlainText() )
        m_noteResource.setProperty( NIE::plainTextContent(), toPlainText() );
}

void NoteEdit::reset()
{
    clear();
    // Create a new note
    m_noteResource = Nepomuk::Resource( QUrl(), PIMO::Note() );
    connect( this, SIGNAL(textChanged()), this, SLOT(slotCreateNote()) );
}

void NoteEdit::slotCreateNote()
{
    save();
    disconnect( this, SIGNAL(textChanged()), this, SLOT(slotCreateNote()) );
}

void NoteEdit::keyPressEvent(QKeyEvent* event)
{
    const QString plainText = toPlainText();
    QChar lastChar;
    if( !plainText.isEmpty() )
        lastChar = plainText.at( plainText.length() - 1 );

    // Do not allow double spaces while typing
    if( event->key() == Qt::Key_Space && lastChar == QChar::fromAscii(' ') ) {
        event->accept();
        return;
    }

    KTextEdit::keyPressEvent( event );
}
