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
#include "notedocument.h"
#include "annotationtextobject.h"
#include "annotationgrouptextobject.h"
#include "person/personcompleter.h"
#include "person/personmodel.h"
#include "annotation/textannotation.h"
#include "annotation/textannotationgroup.h"
#include "annotation/annotator.h"

#include <QtGui/QKeyEvent>
#include <QtGui/QListView>
#include <QtGui/QScrollBar>
#include <QtGui/QFontMetrics>
#include <QtGui/QTextDocumentFragment>
#include <QtGui/QAbstractProxyModel>

#include <Nepomuk/Variant>
#include <Nepomuk/ResourceManager>

#include <Nepomuk/Vocabulary/NIE>
#include <Nepomuk/Vocabulary/PIMO>
#include <Soprano//Vocabulary/NAO>

#include <KDebug>

using namespace Nepomuk::Vocabulary;
using namespace Soprano::Vocabulary;

NoteEdit::NoteEdit(QWidget* parent)
    : KTextEdit(parent)
{
    setCheckSpellingEnabled( true );

    m_document = new NoteDocument( this );
    setDocument( m_document );

    m_completer = new PersonCompleter( this );
    m_completer->setWidget( this );

    connect( m_completer, SIGNAL(activated(QString)), this, SLOT(insertCompletion(QString)) );

    // So that we can render the people differently
    QObject *annotationTextInterfaceObject = new AnnotationTextObject;
    m_document->documentLayout()->registerHandler( AnnotationTextObject::AnnotationTextFormat,
                                                   annotationTextInterfaceObject );
    QObject *annotationGroupTextInterfaceObject = new AnnotationGroupTextObject;
    m_document->documentLayout()->registerHandler( AnnotationGroupTextObject::AnnotationGroupTextFormat,
                                                   annotationGroupTextInterfaceObject );

    // Annotations
    connect( Annotator::instance(), SIGNAL(newAnnotation(Nepomuk::Annotation*)),
             this, SLOT(slotNewAnnotation(Nepomuk::Annotation*)) );
    connect( Annotator::instance(), SIGNAL(finished()),
             this, SLOT(slotAnnotationsFinished()) );
}

NoteEdit::~NoteEdit()
{
}

void NoteEdit::setResource(const Nepomuk::Resource& note)
{
    if( note.isValid() ) {
        reset();

        m_noteResource = note;

        QString htmlContext = m_noteResource.property( NIE::htmlContent() ).toString();
        m_document->setRDFaHtml( htmlContext );
        m_document->setModified( false );

        moveCursor( QTextCursor::End );
    }
}

Nepomuk::Resource NoteEdit::resource() const
{
    return m_noteResource;
}


void NoteEdit::save()
{
    kDebug() << "Saving : " << m_noteResource.resourceUri();
    const QString plainText = m_document->plainText();
    kDebug() << plainText;

    m_noteResource.setProperty( NIE::plainTextContent(), plainText );
    m_noteResource.setProperty( NIE::htmlContent(), m_document->toRDFaHtml() );

    // Get the links in the note. In the future they could be something other than people.
    QSet<QUrl> people = m_document->resources( PIMO::isRelated() );
    m_noteResource.setProperty( PIMO::isRelated(), Nepomuk::Variant(people.toList()) );
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
    //kDebug() << "Word under cursor: " << text;
    if( text.isEmpty() )
        return;

    if( text[0] == QChar::fromAscii('@') ) {
        m_completer->setCompletionPrefix( text.mid( 1) );
    }
    else {
        if( text.length() <= 2 )
            return;
        m_completer->setCompletionPrefix( text );
    }

    // Get the cursor at the start of the tag
    // FIXME: Find a better way!
    QTextCursor origCursor = textCursor();
    QTextCursor tc = textCursor();
    tc.movePosition( QTextCursor::Left, QTextCursor::MoveAnchor, text.length() );
    setTextCursor( tc );
    QRect cr = cursorRect();
    setTextCursor( origCursor );

    cr.setWidth( m_completer->popup()->sizeHintForColumn(0)
                 + m_completer->popup()->horizontalScrollBar()->sizeHint().width() );
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

namespace {
    QChar charOnLeft(const QTextCursor& textCursor) {
        QTextCursor tc(textCursor);
        tc.clearSelection();
        tc.movePosition( QTextCursor::Left, QTextCursor::KeepAnchor );
        QString text = tc.selectedText();
        if( text.length() == 1 )
            return text[0];

        return QChar();
    }
}

void NoteEdit::insertCompletion(const QString& string)
{
    QModelIndex index = m_completer->popup()->currentIndex();
    QUrl resourceUri = index.data( PersonModel::UriRole ).toUrl();
    QString compPrefix = m_completer->completionPrefix();

    QTextCursor tc = textCursor();
    tc.anchor();
    tc.movePosition( QTextCursor::Left, QTextCursor::KeepAnchor, compPrefix.length() );

    // Remove the '@' if present
    QChar leftChar = charOnLeft( tc );
    if( leftChar == QChar::fromAscii('@') )
        tc.movePosition( QTextCursor::Left, QTextCursor::KeepAnchor );

    tc.removeSelectedText();

    QTextCharFormat personFormat;
    personFormat.setObjectType( AnnotationTextObject::AnnotationTextFormat );

    personFormat.setProperty( AnnotationTextObject::AnnotationText, string );
    personFormat.setProperty( AnnotationTextObject::AnnotationUri, resourceUri );
    personFormat.setProperty( AnnotationTextObject::AnnotationProperty, PIMO::isRelated() );

    tc.insertText( QString(QChar::ObjectReplacementCharacter), personFormat );
    setTextCursor( tc );
}

void NoteEdit::slotNewAnnotation(Nepomuk::Annotation* annotation)
{
    kDebug();
    TextAnnotation* textAnnotation = dynamic_cast<TextAnnotation*>( annotation );
    if( !textAnnotation ) {
        return;
    }

    QString plainText = m_noteResource.property(NIE::plainTextContent()).toString();
    kDebug() << "plugin: " << plainText.mid( textAnnotation->position(), textAnnotation->length() );

    //TODO: make sure these objects are sorted based on startPos
    QList< NoteDocument::Annotation > objectList = m_document->annotations();

    bool valid = true;
    foreach( const NoteDocument::Annotation& ann, objectList ) {
        if( textAnnotation->position() == ann.startPos && textAnnotation->length() <= ann.text.length() ) {
            kDebug() << "Already exists";
            valid = false;
            break;
        }

        if( textAnnotation->position() > ann.startPos ) {
            int pos = textAnnotation->position();

            // The +1 is cause each object takes 1 char - QChar::ObjectReplacementCharacter
            pos -= ann.text.length();
            pos += 1;

            textAnnotation->setPosition( pos );
        }
    }

    if( valid ) {
        m_annotations.insert( textAnnotation->group(), textAnnotation );
    }
}

void NoteEdit::insertAnnotation(TextAnnotation* ann)
{
    int pos = ann->position();
    int len = ann->length();

    QTextCursor tc = textCursor();
    tc.beginEditBlock();
    tc.movePosition( QTextCursor::Start );
    tc.movePosition( QTextCursor::Right, QTextCursor::MoveAnchor, pos );
    tc.movePosition( QTextCursor::Right, QTextCursor::KeepAnchor, len );

    QString text = tc.selectedText();
    if( text != ann->text() ) {
        kWarning() << "Warning. Buggy code. Annotations positions are not in sync.";
        kWarning() << "Existing " << text << " vs " << ann->text();
        return;
    }

    QTextCharFormat annotationFormat;
    annotationFormat.setObjectType( AnnotationTextObject::AnnotationTextFormat );

    annotationFormat.setProperty( AnnotationTextObject::AnnotationText, text );
    annotationFormat.setProperty( AnnotationTextObject::AnnotationUri, ann->object().resourceUri() );
    annotationFormat.setProperty( AnnotationTextObject::AnnotationProperty, ann->property() );

    tc.removeSelectedText();
    tc.insertText( QString(QChar::ObjectReplacementCharacter), annotationFormat );
    tc.endEditBlock();
    setTextCursor( tc );

    // Update the positions of all the pending TextAnnotations
    QHashIterator<int, TextAnnotation*> iter( m_annotations );
    while( iter.hasNext() ) {
        TextAnnotation* annotation = iter.next().value();

        int pos = annotation->position();
        if( pos > ann->position() ) {
            pos -= ann->length() - 1;
            annotation->setPosition( pos );
        }
    }

}

void NoteEdit::insertGroupAnnotation(TextAnnotationGroup* tag)
{
    if( tag->annotations().isEmpty() )
        return;

    TextAnnotation* ann = tag->annotations().first();

    int pos = ann->position();
    int len = ann->length();

    QTextCursor tc = textCursor();
    tc.beginEditBlock();
    tc.movePosition( QTextCursor::Start );
    tc.movePosition( QTextCursor::Right, QTextCursor::MoveAnchor, pos );
    tc.movePosition( QTextCursor::Right, QTextCursor::KeepAnchor, len );

    QString text = tc.selectedText();
    if( text != ann->text() ) {
        kWarning() << "Warning. Buggy code. Annotations positions are not in sync.";
        kWarning() << "Existing " << text << " vs " << ann->text();
        return;
    }

    QTextCharFormat annotationFormat;
    annotationFormat.setObjectType( AnnotationGroupTextObject::AnnotationGroupTextFormat );

    annotationFormat.setProperty( AnnotationGroupTextObject::AnnotationText, text );
    annotationFormat.setProperty( AnnotationGroupTextObject::AnnotationData, qVariantFromValue(tag) );

    tc.removeSelectedText();
    tc.insertText( QString(QChar::ObjectReplacementCharacter), annotationFormat );
    tc.endEditBlock();
    setTextCursor( tc );

    // Update the positions of all the pending TextAnnotations
    QHashIterator<int, TextAnnotation*> iter( m_annotations );
    while( iter.hasNext() ) {
        TextAnnotation* annotation = iter.next().value();

        int pos = annotation->position();
        if( pos > ann->position() ) {
            pos -= ann->length() - 1;
            annotation->setPosition( pos );
        }
    }

}

void NoteEdit::slotAnnotationsFinished()
{
    QList<int> keys = m_annotations.keys();
    foreach( int k, keys ) {
        QList<TextAnnotation*> annotations = m_annotations.values( k );
        m_annotations.remove( k );

        if( annotations.length() == 1 ) {
            TextAnnotation* textAnnotation = annotations.first();
            insertAnnotation( textAnnotation );
        }
        else {
            kDebug() << "GOT MULTIPLE";
            TextAnnotationGroup* tag = new TextAnnotationGroup( annotations );
            insertGroupAnnotation( tag );

            //FIXME: Now what?
            // We need to sort by relevance and have some kind of cut off, and show options to the
            // user
        }
    }
    m_annotations.clear();
}
