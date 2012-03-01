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

#include <QtGui/QTextBlock>
#include <QtGui/QTextFragment>

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

            //TODO: Check if special kind of fragment
            paragraph.append( fragment.text() );
        }
        paragraph.append("</p>");

        html.append( paragraph );
        textBlock = textBlock.next();
    }

    html.append("</body></html>");
    return html;
}

void NoteDocument::setRDFaHtml(const QString& html)
{
    //FIXME: Implement me!
    setHtml( html );
}
