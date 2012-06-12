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

#include "../annotationrequest.h"
#include "../simpleannotation.h"

#include <Nepomuk2/Resource>
#include <Nepomuk2/Variant>

#include <Nepomuk2/Query/Query>
#include <Nepomuk2/Query/ResourceTypeTerm>

#include <Nepomuk2/Vocabulary/PIMO>
#include <Nepomuk2/Vocabulary/NIE>

#include <KDebug>
#include <QTimer>

using namespace Nepomuk2::Vocabulary;

PersonAnnotationPlugin::PersonAnnotationPlugin( QObject* parent, const QVariantList& )
    : Nepomuk2::AnnotationPlugin( parent )
{
    m_personModel = new PersonModel( this );

    Nepomuk2::Query::ResourceTypeTerm term(PIMO::Person());
    Nepomuk2::Query::Query q(term);
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

namespace {
    QSet<TextAnnotation*> textAnnotationIntersection(const QSet<TextAnnotation*> &set1,
                                                     const QSet<TextAnnotation*> &set2) {

        QSet<TextAnnotation*> finalSet;
        foreach( TextAnnotation* t1, set1 ) {
            foreach( const TextAnnotation* t2, set2 ) {
                if( t1->property() == t2->property() && t1->object() == t2->object() )
                    finalSet.insert( t1 );
            }
        }

        return finalSet;
    }
}

void PersonAnnotationPlugin::doGetPossibleAnnotations( const Nepomuk2::AnnotationRequest& request )
{
    QString text = request.filter();
    if( !text.isEmpty() ) {

        QList< Person > people = matchingPeople( text );
        foreach( const Person& person, people ) {
            // FIXME: You typically want to give the exact string
            kDebug() << person.nickName();
            Nepomuk2::SimpleAnnotation *ann = new Nepomuk2::SimpleAnnotation( PIMO::isRelated(), person.uri() );
            addNewAnnotation( ann );
        }

        return;
    }

    // Go through the contents
    const QString origText = request.resource().property( NIE::plainTextContent() ).toString();
    text = origText;
    if( text.isEmpty() )
        return;

    //TODO: Implement a better tokenizer
    QRegExp regex("[^\\w]");
    text.replace( regex, QString::fromLatin1(" ") );
    text.replace( '\n', ' ' );

    QSet<TextAnnotation*> lastAnnotations;
    int lastStart = -1;

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

            QSet<TextAnnotation*> textAnnotations = createTextAnnotations( group++, word, start, len );

            // Check if last and current can be grouped
            QSet<TextAnnotation*> intersection = textAnnotationIntersection( lastAnnotations, textAnnotations );
            if( intersection.isEmpty() ) {
                addAnnotations( lastAnnotations );

                lastStart = start;
                start = -1;
                lastAnnotations = textAnnotations;
                continue;
            }
            else {
                int len = end - lastStart + 1;
                QString text = origText.mid( lastStart, len );

                QMutableSetIterator<TextAnnotation*> iter( intersection );
                while( iter.hasNext() ) {
                    TextAnnotation* ann = iter.next();
                    ann->setPosition( lastStart );
                    ann->setLength( end - lastStart + 1 );
                    ann->setText( text );
                }

                start = -1;
                lastAnnotations = intersection;
            }
        }
    }

    int len = text.length() - start + 1;
    QString word = text.mid( start, len );

    addAnnotations( lastAnnotations );
    addAnnotations( createTextAnnotations( group++, word, start, len ) );

    emitFinished();
}

QSet<TextAnnotation*> PersonAnnotationPlugin::createTextAnnotations(int group, const QString& word, int start, int len)
{
    QSet<TextAnnotation*> set;

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

        set.insert( ann );
    }

    return set;
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

void PersonAnnotationPlugin::addAnnotations(const QSet< TextAnnotation* > annotations)
{
    foreach( TextAnnotation* annotation, annotations )
        addNewAnnotation( annotation );
}



NEPOMUK_EXPORT_ANNOTATION_PLUGIN( PersonAnnotationPlugin, "notably_personannotationplugin" )

