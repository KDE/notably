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
    if( text.isEmpty() ) {
        return;
        //text = request.resource().property( NIE::plainTextContent() ).toString();
    }

    m_filterModel->setFilterFixedString( text );

    for( int i=0; i<m_filterModel->rowCount(); i++ ) {
        QModelIndex index = m_filterModel->mapToSource( m_filterModel->index( i, 0 ) );
        if( !index.isValid() ) {
            break;
        }

        const QUrl personUri = index.data( PersonModel::UriRole ).toUrl();
        Person person(personUri);

        // FIXME: You typically want to give the exact string
        kDebug() << person.nickName();
        Nepomuk::SimpleAnnotation *ann = new Nepomuk::SimpleAnnotation( PIMO::isRelated(), person.uri() );
        addNewAnnotation( ann );
    }

}

NEPOMUK_EXPORT_ANNOTATION_PLUGIN( PersonAnnotationPlugin, "notably_personannotationplugin" )

