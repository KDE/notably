/*
   Copyright (c) 2012 Vishesh Handa <me@vhanda.in>

   This library is free software; you can redistribute it and/or
   modify it under the terms of the GNU Lesser General Public
   License as published by the Free Software Foundation; either
   version 2.1 of the License, or (at your option) any later version.

   This library is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
   Lesser General Public License for more details.

   You should have received a copy of the GNU Lesser General Public License
   along with this library; see the file COPYING.LIB.  If not, write to
   the Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor,
   Boston, MA 02110-1301, USA.
*/

#include "personannotationplugin.h"
#include "../textannotation.h"

#include <nepomuk/annotationrequest.h>
#include <nepomuk/simpleannotation.h>

#include <Nepomuk/Resource>
#include <Nepomuk/Variant>

#include <Nepomuk/Query/Query>
#include <Nepomuk/Query/ResourceTypeTerm>

#include <Nepomuk/Vocabulary/PIMO>
#include <Nepomuk/Vocabulary/NIE>

#include <KDebug>
#include <QTimer>

using namespace Nepomuk::Vocabulary;

PersonAnnotationPlugin::PersonAnnotationPlugin( QObject* parent, const QVariantList& )
    : Nepomuk::AnnotationPlugin( parent )
{
    m_personModel = new PersonModel( this );

    Nepomuk::Query::ResourceTypeTerm term(PIMO::Person());
    Nepomuk::Query::Query q(term);
    q.setLimit( 5 );
    m_personModel->setQuery( q );

    m_filterModel = new QSortFilterProxyModel( this );
    m_filterModel->setSourceModel( m_personModel );
    m_filterModel->setFilterCaseSensitivity( Qt::CaseInsensitive );
    m_filterModel->setDynamicSortFilter( true );
}


PersonAnnotationPlugin::~PersonAnnotationPlugin()
{
}

void PersonAnnotationPlugin::doGetPossibleAnnotations( const Nepomuk::AnnotationRequest& request )
{
    QString text = request.filter();
    if( !text.isEmpty() ) {

        QList< Person > people = matchingPeople( text );
        foreach( const Person& person, people ) {
            // FIXME: You typically want to give the exact string
            kDebug() << person.nickName();
            Nepomuk::SimpleAnnotation *ann = new Nepomuk::SimpleAnnotation( PIMO::isRelated(), person.uri() );
            addNewAnnotation( ann );
        }

        return;
    }

    // Go through the contents
    text = request.resource().property( NIE::plainTextContent() ).toString();
    if( text.isEmpty() )
        return;

    //TODO: Implement a better tokenizer
    QRegExp regex("[^\\w]");
    text.replace( regex, QString::fromLatin1(" ") );
    text.replace( '\n', ' ' );

    int start = -1;
    int end = 0;
    int group = 0;
    for( int i=0; i<text.length(); i++ ) {
        if( start == -1 ) {
            if( text[i].isLetter() ) {
                start = i;
            }
            else {
                continue;
            }
        }

        if( text[i].isSpace() ) {
            end = i-1;

            int len = end - start + 1;
            QString word = text.mid( start, len );

            createTextAnnotations( group++, word, start, len );
            start = -1;
        }
    }

    int len = text.length() - start + 1;
    QString word = text.mid( start, len );

    createTextAnnotations( group++, word, start, len );
}

void PersonAnnotationPlugin::createTextAnnotations(int group, const QString& word, int start, int len)
{
    QModelIndexList indexList = matchingIndexes( word );
    foreach( const QModelIndex& index, indexList ) {
        const QUrl uri = index.data( PersonModel::UriRole ).toUrl();
        const QString display = index.data( Qt::DisplayRole ).toString();

        //FIXME: We need something better than this
        bool meetsCritera = false;
        QStringList names = display.split( QChar::fromAscii(' '), QString::SkipEmptyParts );
        foreach( const QString& name, names ) {
            if( name.length() == word.length() ) {
                meetsCritera = true;
                break;
            }
        }

        if( !meetsCritera )
            continue;

        TextAnnotation* ann = new TextAnnotation( start, len, PIMO::isRelated(), uri );
        ann->setGroup( group );
        ann->setText( word );

        addNewAnnotation( ann );
    }

    emitFinished();
}

QList< Person > PersonAnnotationPlugin::matchingPeople(const QString& text)
{
    QList<Person> people;
    m_filterModel->setFilterFixedString( text );

    for( int i=0; i<m_filterModel->rowCount(); i++ ) {
        QModelIndex index = m_filterModel->mapToSource( m_filterModel->index( i, 0 ) );
        if( !index.isValid() )
            break;

        const QUrl personUri = index.data( PersonModel::UriRole ).toUrl();
        people << Person(personUri);
    }

    return people;
}

QModelIndexList PersonAnnotationPlugin::matchingIndexes(const QString& text)
{
    QModelIndexList indexList;
    m_filterModel->setFilterFixedString( text );

    for( int i=0; i<m_filterModel->rowCount(); i++ ) {
        QModelIndex index = m_filterModel->mapToSource( m_filterModel->index( i, 0 ) );
        if( !index.isValid() )
            break;

        indexList << index;
    }

    return indexList;
}


NEPOMUK_EXPORT_ANNOTATION_PLUGIN( PersonAnnotationPlugin, "notably_personannotationplugin" )

