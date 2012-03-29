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


#include "notedocument.h"
#include "annotationtextobject.h"
#include "annotationgrouptextobject.h"

#include <QtGui/QTextBlock>
#include <QtGui/QTextFragment>
#include <QtGui/QTextDocumentFragment>

#include <QtXml/QDomDocument>
#include <QtXml/QDomNodeList>

#include <QtCore/QUrl>

#include <Nepomuk/Vocabulary/PIMO>
#include <KDebug>

using namespace Nepomuk::Vocabulary;

NoteDocument::NoteDocument(QObject* parent): QTextDocument(parent)
{
    connect( this, SIGNAL(contentsChanged()), this, SLOT(slotContentsChanged()) );
}

NoteDocument::~NoteDocument()
{

}

void NoteDocument::slotContentsChanged()
{
    m_htmlContent.clear();
}

QString NoteDocument::toRDFaHtml() const
{
    if( !m_htmlContent.isEmpty() )
        return m_htmlContent;

    QString html = QString::fromLatin1("<html><body>");

    QTextBlock textBlock = begin();
    while( textBlock.isValid() ) {
        QString paragraph = QLatin1String("<p>");
        QTextBlock::iterator it = textBlock.begin();
        for( ; !it.atEnd(); it++ ) {
            QTextFragment fragment = it.fragment();
            QTextCharFormat format = fragment.charFormat();

            QString txt = fragment.text();
            const bool isObject = txt.contains(QChar::ObjectReplacementCharacter);
            const bool isGroupObject = format.hasProperty( AnnotationGroupTextObject::AnnotationData );

            if( isObject && !isGroupObject ) {
                const QString text = format.property( AnnotationTextObject::AnnotationText ).toString();
                const QUrl uri = format.property( AnnotationTextObject::AnnotationUri ).toUrl();
                const QUrl prop = format.property( AnnotationTextObject::AnnotationProperty ).toUrl();

                QString string = QString::fromLatin1("<span rel='%1' resource='%2'>%3</span>")
                               .arg( prop.toString(), uri.toString(), text );
                paragraph.append( string );

                // They could be viable text in the fragment.
                txt.remove(QChar::ObjectReplacementCharacter);
                if( !txt.isEmpty() )
                    paragraph.append( txt );
            }
            else if( isObject && isGroupObject ) {
                const QString string = format.property( AnnotationGroupTextObject::AnnotationText ).toString();
                paragraph.append( string );

                // They could be viable text in the fragment.
                txt.remove(QChar::ObjectReplacementCharacter);
                if( !txt.isEmpty() )
                    paragraph.append( txt );
            }
            else {
                paragraph.append( fragment.text() );
            }
        }
        paragraph.append("</p>");

        html.append( paragraph );
        textBlock = textBlock.next();
    }

    html.append("</body></html>");

    NoteDocument* doc = const_cast<NoteDocument*>(this);
    doc->m_htmlContent = html;

    return html;
}

namespace {
    QTextCharFormat personFormat(const QUrl& resourceUri, const QString& name) {
        QTextCharFormat format;
        format.setObjectType( AnnotationTextObject::AnnotationTextFormat );

        format.setProperty( AnnotationTextObject::AnnotationText, name );
        format.setProperty( AnnotationTextObject::AnnotationUri, resourceUri );
        format.setProperty( AnnotationTextObject::AnnotationProperty, PIMO::isRelated() );

        return format;
    }
}
void NoteDocument::setRDFaHtml(const QString& html)
{
    QDomDocument document;
    document.setContent( html );

    QDomNodeList nodeList = document.elementsByTagName(QLatin1String("body"));
    if( nodeList.isEmpty() ) {
        kWarning() << "Could not find the html body. Exiting";
        return;
    }
    QDomElement bodyElement = nodeList.at(0).toElement();

    clear();
    QTextCursor cursor( this );

    QDomNodeList paragraphs = bodyElement.elementsByTagName(QLatin1String("p"));
    for( int i=0; i<paragraphs.size(); i++ ) {
        QDomNode para = paragraphs.item(i);

        QDomNode node = para.firstChild();
        while( !node.isNull() ) {
            if( node.isText() ) {
                cursor.insertFragment( QTextDocumentFragment::fromPlainText( node.toText().data() ) );
            }
            else if( node.isElement() ) {
                QDomElement span = node.toElement();
                QString rel = span.attribute(QLatin1String("rel"));
                QString resource = span.attribute(QLatin1String("resource"));
                QString name = span.text();

                QTextCharFormat pf = personFormat( resource, name );
                cursor.insertText( QString(QChar::ObjectReplacementCharacter), pf );
            }

            node = node.nextSibling();
        }

        cursor.insertBlock();
    }
}

QString NoteDocument::plainText() const
{
    const QString html = toRDFaHtml();

    QTextDocument doc;
    doc.setHtml( html );

    QString text = doc.toPlainText();
    //vHanda: toPlainText() strips all the empty paragrphs. We need them.
    text.replace("\n", "\n\n");

    return text;
}

QSet< QUrl > NoteDocument::resources(const QUrl& property)
{
    QSet<QUrl> uris;

    QTextBlock textBlock = begin();
    while( textBlock.isValid() ) {
        QTextBlock::iterator it = textBlock.begin();
        for( ; !it.atEnd(); it++ ) {
            QTextFragment fragment = it.fragment();

            const QString txt = fragment.text();
            const bool isObject = txt.contains(QChar::ObjectReplacementCharacter);

            if( isObject ) {
                QTextCharFormat format = fragment.charFormat();
                const QUrl uri = format.property( AnnotationTextObject::AnnotationUri ).toUrl();
                const QUrl prop = format.property( AnnotationTextObject::AnnotationProperty ).toUrl();

                if( prop == property )
                    uris << uri;
            }
        }

        textBlock = textBlock.next();
    }

    return uris;
}

QList< NoteDocument::Annotation > NoteDocument::annotations()
{
    QList<Annotation> annotations;

    QTextBlock textBlock = begin();
    while( textBlock.isValid() ) {
        QTextBlock::iterator it = textBlock.begin();
        for( ; !it.atEnd(); it++ ) {
            QTextFragment fragment = it.fragment();

            const QString txt = fragment.text();
            const bool isObject = txt.contains(QChar::ObjectReplacementCharacter);

            if( isObject ) {
                QTextCharFormat format = fragment.charFormat();

                Annotation ann;
                ann.startPos = fragment.position();
                ann.format = format;
                ann.text = format.property( AnnotationTextObject::AnnotationText ).toString();

                annotations << ann;
            }
        }

        textBlock = textBlock.next();
    }

    return annotations;
}

