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


#include "notesmodel.h"

#include <Nepomuk/Query/Query>
#include <Nepomuk/Query/ResourceTypeTerm>
#include <Nepomuk/Query/ComparisonTerm>
#include <Nepomuk/Query/QueryServiceClient>

#include <Nepomuk/Types/Class>
#include <Nepomuk/Variant>

#include <Nepomuk/Vocabulary/PIMO>
#include <Soprano/Vocabulary/NAO>

#include <KDebug>

using namespace Nepomuk::Vocabulary;
using namespace Soprano::Vocabulary;

NotesModel::NotesModel(QObject* parent): SimpleResourceModel(parent)
{
    Nepomuk::Query::QueryServiceClient *client = new Nepomuk::Query::QueryServiceClient( this );
    connect( client, SIGNAL(newEntries(QList<Nepomuk::Query::Result>)),
             this, SLOT(addResults(QList<Nepomuk::Query::Result>)) );
    connect( client, SIGNAL(finishedListing()), client, SLOT(deleteLater()) );

    Nepomuk::Query::ResourceTypeTerm typeTerm( Nepomuk::Types::Class( PIMO::Note() ) );
    Nepomuk::Query::ComparisonTerm compTerm( NAO::lastModified(), Nepomuk::Query::Term() );
    compTerm.setSortWeight( 1, Qt::DescendingOrder );

    Nepomuk::Query::Query query( typeTerm && compTerm );
    client->query( query );
}

NotesModel::~NotesModel()
{

}