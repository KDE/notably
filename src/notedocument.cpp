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
#include "persontextobject.h"

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

}

NoteDocument::~NoteDocument()
{

}

QString NoteDocument::toRDFaHtml() const
{
    QString html = QString::fromLatin1("<html><body>");

    QTextBlock textBlock = begin();
    while( textBlock.isValid() ) {
        QString paragraph = QLatin1String("<p>");
        QTextBlock::iterator it = textBlock.begin();
        for( ; !it.atEnd(); it++ ) {
            QTextFragment fragment = it.fragment();
            QTextCharFormat format = fragment.charFormat();

            const QString txt = fragment.text();
            const bool isObject = txt.contains(QChar::ObjectReplacementCharacter);

            if( isObject ) {
                const QString name = format.property( PersonTextObject::PersonName ).toString();
                const QUrl uri = format.property( PersonTextObject::PersonUri ).toUrl();

                QString text = QString::fromLatin1("<span rel='%1' resource='%2'>%3</span>")
                               .arg( PIMO::isRelated().toString(),
                                     uri.toString(),
                                     name );
                paragraph.append( text );
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
    return html;
}

namespace {
    QTextCharFormat personFormat(const QUrl& resourceUri, const QString& name) {
        QTextCharFormat format;
        format.setObjectType( PersonTextObject::PersonTextFormat );

        format.setProperty( PersonTextObject::PersonName, name );
        format.setProperty( PersonTextObject::PersonUri, resourceUri );

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
                //FIXME: Check for properties
                QTextCharFormat format = fragment.charFormat();
                const QUrl uri = format.property( PersonTextObject::PersonUri ).toUrl();

                uris << uri;
            }
        }

        textBlock = textBlock.next();
    }

    return uris;
}
