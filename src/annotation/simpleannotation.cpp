/* This file is part of the Nepomuk Project
   Copyright (c) 2008-2009 Sebastian Trueg <trueg@kde.org>

   This library is free software; you can redistribute it and/or
   modify it under the terms of the GNU Library General Public
   License version 2 as published by the Free Software Foundation.

   This library is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
   Library General Public License for more details.

   You should have received a copy of the GNU Library General Public License
   along with this library; see the file COPYING.LIB.  If not, write to
   the Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor,
   Boston, MA 02110-1301, USA.
*/

#include "simpleannotation.h"

#include <Soprano/Model>
#include <Soprano/Node>
#include <Soprano/QueryResultIterator>
#include <Soprano/Vocabulary/NAO>

#include <Nepomuk2/Variant>
#include <Nepomuk2/Types/Property>
#include <nepomuk2/resourcemanager.h>

#include <QtCore/QUrl>

#include <kicon.h>
#include <kdebug.h>

class Nepomuk2::SimpleAnnotation::Private
{
public:
    Nepomuk2::Types::Property property;
    Variant value;
};


Nepomuk2::SimpleAnnotation::SimpleAnnotation( QObject* parent )
    : Annotation( parent ),
      d( new Private() )
{
}


Nepomuk2::SimpleAnnotation::SimpleAnnotation( const Types::Property& prop, const Variant& object, QObject* parent )
    : Annotation( parent ),
      d( new Private() )
{
    d->property = prop;
    d->value = object;
    Q_ASSERT( !object.isList() );
}


Nepomuk2::SimpleAnnotation::~SimpleAnnotation()
{
    delete d;
}


void Nepomuk2::SimpleAnnotation::setProperty( const Types::Property& p )
{
    d->property = p;
    if ( d->property.icon().isNull() )
        setIcon( KIcon( "nepomuk" ) );
    else
        setIcon( d->property.icon() );
}


void Nepomuk2::SimpleAnnotation::setValue( const Variant& value )
{
    Q_ASSERT( !value.isList() );
    d->value = value;
}


Nepomuk2::Types::Property Nepomuk2::SimpleAnnotation::property() const
{
    return d->property;
}


Nepomuk2::Variant Nepomuk2::SimpleAnnotation::value() const
{
    return d->value;
}


bool Nepomuk2::SimpleAnnotation::equals( Annotation* other ) const
{
    if ( SimpleAnnotation* sa = qobject_cast<SimpleAnnotation*>( other ) ) {
        return( sa->d->property == d->property &&
                sa->d->value == d->value );
    }

    return Annotation::equals( other );
}


bool Nepomuk2::SimpleAnnotation::exists( Resource res ) const
{
    return res.hasProperty( d->property, d->value );
}


int Nepomuk2::SimpleAnnotation::occurenceCount( const QDateTime& from, const QDateTime& to ) const
{
    // FIXME: how should we treat literal annotations here? It is probably best to only count the usage of the property in that case.
    // FIXME: hanldle the new property case
    QString query = QString( "?r %1 %2 .")
                    .arg( Soprano::Node( d->property.uri() ).toN3() )
                    .arg( Soprano::Node( d->value.toResource().uri() ).toN3() );
    if( from.isValid() || to.isValid() ) {
        query = QString("graph ?g { %1 } . ?g %2 ?date . ")
                .arg( query )
                .arg( Soprano::Node( Soprano::Vocabulary::NAO::created() ).toN3() );
        if( from.isValid() ) {
            query += QString("FILTER(?date >= %1) . ")
                     .arg( Soprano::Node(from).toN3() );
        }
        if( to.isValid() ) {
            query += QString("FILTER(?date <= %1) . ")
                     .arg( Soprano::Node(to).toN3() );
        }
    }
    query = "select ?r where { " + query + "}";

    // why oh why doesn't sparql have a count method?
    Soprano::QueryResultIterator it = ResourceManager::instance()->mainModel()->executeQuery( query, Soprano::Query::QueryLanguageSparql );
    int cnt = 0;
    while( it.next() ) {
        ++cnt;
    }
    return cnt;
}


QDateTime Nepomuk2::SimpleAnnotation::lastUsed() const
{
    // FIXME: hanldle the new property case
    Soprano::QueryResultIterator it =
        ResourceManager::instance()->mainModel()->executeQuery( QString("select ?date where { graph ?g { ?r %1 %2 . }  . ?g %3 ?date . } ORDER BY DESC(?date) LIMIT 1")
                                                                .arg( Soprano::Node( d->property.uri() ).toN3() )
                                                                .arg( Soprano::Node( d->value.toResource().uri() ).toN3() )
                                                                .arg( Soprano::Node( Soprano::Vocabulary::NAO::created() ).toN3() ),
                                                                Soprano::Query::QueryLanguageSparql );
    if( it.next() ) {
        return it.binding( "date" ).literal().toDateTime();
    }
    else {
        return QDateTime();
    }
}


QDateTime Nepomuk2::SimpleAnnotation::firstUsed() const
{
    // FIXME: hanldle the new property case
    Soprano::QueryResultIterator it =
        ResourceManager::instance()->mainModel()->executeQuery( QString("select ?date where { graph ?g { ?r %1 %2 . }  . ?g %3 ?date . } ORDER BY ASC(?date) LIMIT 1")
                                                                .arg( Soprano::Node( d->property.uri() ).toN3() )
                                                                .arg( Soprano::Node( d->value.toResource().uri() ).toN3() )
                                                                .arg( Soprano::Node( Soprano::Vocabulary::NAO::created() ).toN3() ),
                                                                Soprano::Query::QueryLanguageSparql );
    if( it.next() ) {
        return it.binding( "date" ).literal().toDateTime();
    }
    else {
        return QDateTime();
    }
}


void Nepomuk2::SimpleAnnotation::doCreate( Resource res )
{
    kDebug() << res.uri();
    if ( d->property.isValid() ) {
        // TODO: here we ignore possible type clashes
        if ( d->property.maxCardinality() == 1 ) {
            res.setProperty( d->property.uri(), d->value );
        }
        else {
            Variant vals = res.property( d->property.uri() );
            vals.append( d->value );
            res.setProperty( d->property.uri(), vals );
        }
    }
    else {
        kDebug() << "invalid property!";
    }

    emitFinished();
}

#include "simpleannotation.moc"
