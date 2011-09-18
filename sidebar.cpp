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


#include "sidebar.h"
#include "noteitemdelegate.h"

#include <QtGui/QLabel>
#include <QtGui/QListView>

#include <Nepomuk/Query/Query>
#include <Nepomuk/Query/ResourceTypeTerm>
#include <Nepomuk/Query/QueryServiceClient>

#include <Nepomuk/Types/Class>
#include <Nepomuk/Utils/SimpleResourceModel>

#include <Nepomuk/Vocabulary/PIMO>
#include <Soprano/Vocabulary/NAO>

#include <KDebug>

using namespace Nepomuk::Vocabulary;
using namespace Soprano::Vocabulary;

Sidebar::Sidebar(QWidget* parent, Qt::WindowFlags f)
    : QListView(parent)
{
    Nepomuk::Utils::SimpleResourceModel *model = new Nepomuk::Utils::SimpleResourceModel( this );

    Nepomuk::Query::QueryServiceClient *client = new Nepomuk::Query::QueryServiceClient( this );
    connect( client, SIGNAL(newEntries(QList<Nepomuk::Query::Result>)),
             model, SLOT(addResults(QList<Nepomuk::Query::Result>)) );

    Nepomuk::Query::ResourceTypeTerm typeTerm( Nepomuk::Types::Class( PIMO::Note() ) );
    Nepomuk::Query::Query query( typeTerm );

    //FIXME: Find a better way of doing this.
    Nepomuk::Query::Query::RequestProperty rp( NAO::lastModified(), false );
    query.addRequestProperty( rp );

    QString sparql = query.toSparqlQuery() + QLatin1String(" order by desc(?reqProp1)");
    client->sparqlQuery( sparql );

    //QListView *view = new QListView( this );
    //setEnabled( false );
    setModel( model );
    setItemDelegate( new NoteItemDelegate( this ) );

    //QLabel *label = new QLabel( this );
    //label->setText("daffffffffffffffff");
}

Sidebar::~Sidebar()
{

}