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


#include "tagcompleter.h"

#include <Nepomuk/Query/Query>
#include <Nepomuk/Query/ResourceTypeTerm>
#include <Nepomuk/Query/QueryServiceClient>

#include <Nepomuk/Types/Class>
#include <Nepomuk/Utils/SimpleResourceModel>

#include <Soprano/Vocabulary/NAO>

using namespace Soprano::Vocabulary;

TagCompleter::TagCompleter(QObject* parent): QCompleter(parent)
{
    Nepomuk::Utils::SimpleResourceModel *model = new Nepomuk::Utils::SimpleResourceModel( this );

    setModel( model );
    setCaseSensitivity( Qt::CaseInsensitive );
    setCompletionColumn( 0 );

    Nepomuk::Query::ResourceTypeTerm typeTerm( Nepomuk::Types::Class( NAO::Tag() ) );
    Nepomuk::Query::Query query( typeTerm );

    Nepomuk::Query::QueryServiceClient *client = new Nepomuk::Query::QueryServiceClient( this );
    connect( client, SIGNAL(newEntries(QList<Nepomuk::Query::Result>)),
             model, SLOT(addResults(QList<Nepomuk::Query::Result>)) );

    client->query( query );
}

TagCompleter::~TagCompleter()
{

}
