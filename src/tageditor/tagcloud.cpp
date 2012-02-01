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


#include "tagcloud.h"

#include <Nepomuk/Query/Query>
#include <Nepomuk/Query/ResourceTypeTerm>
#include <Nepomuk/Query/QueryServiceClient>

#include <Nepomuk/Types/Class>
#include <Nepomuk/Utils/SimpleResourceModel>
#include <Nepomuk/ResourceManager>
#include <KDebug>

#include <Soprano/Model>
#include <Soprano/QueryResultIterator>
#include <Soprano/Vocabulary/NAO>

using namespace Soprano::Vocabulary;

TagCloud::TagCloud(QWidget* parent): QTextBrowser(parent)
{
    m_minFontSize = 25;
    m_maxFontSize = 70;
    m_spacing = 4;

    connect( this, SIGNAL(anchorClicked(QUrl)), this, SLOT(slotAnchorClicked(QUrl)) );

    const QString query = QString::fromLatin1("select ?r (select count(*) where { ?t nao:hasTag ?r }) as ?count"
                                              " where { ?r a nao:Tag . } order by desc(?count)");

    Soprano::Model* model = Nepomuk::ResourceManager::instance()->mainModel();
    Soprano::QueryResultIterator it = model->executeQuery( query, Soprano::Query::QueryLanguageSparql );
    QHash<QString, int> tagHash;
    int total = 0;
    while( it.next() ) {
        QUrl uri = it["r"].uri();
        QString tagLabel = Nepomuk::Resource(uri).identifiers().first();
        int freq = it["count"].literal().toInt();

        tagHash.insert( tagLabel, freq );
        total += freq;
    }

    QHash<QString, int>::const_iterator iter = tagHash.constBegin();
    for(; iter != tagHash.constEnd(); iter++ ) {
        m_hashMap.insert( iter.key(), iter.value() * 1.0 / total );
    }
    createBrowser();
}

void TagCloud::createBrowser()
{
    QString html("<html><body>");

    QMap<QString, float>::const_iterator iter = m_hashMap.constBegin();
    for( ; iter!=m_hashMap.constEnd(); iter++ ) {
        int fontSize = m_minFontSize + (m_maxFontSize - m_minFontSize) * iter.value();

        QString htmlTag = QString::fromLatin1("<a href='%1'><font style='font-size: %2px'>%3"
                                              "</font></a>")
                          .arg( iter.key(), QString::number(fontSize), iter.key() );
        html.append( htmlTag + QString( m_spacing, ' ' ) );
    }

    html.append("</body></html>");
    setHtml( html );
}

void TagCloud::slotAnchorClicked(const QUrl& url)
{
    kDebug() << url;
}
