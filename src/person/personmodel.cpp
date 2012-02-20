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


#include "personmodel.h"

#include <KDebug>

#include <Nepomuk/Query/Query>
#include <Nepomuk/Query/QueryServiceClient>
#include <Nepomuk/Query/ResourceTypeTerm>
#include <Nepomuk/Vocabulary/PIMO>

using namespace Nepomuk::Vocabulary;

PersonModel::PersonModel(QObject* parent): QAbstractListModel(parent)
{
    Nepomuk::Query::ResourceTypeTerm term(PIMO::Person());
    Nepomuk::Query::Query q(term);

    Nepomuk::Query::QueryServiceClient *client = new Nepomuk::Query::QueryServiceClient(this);
    connect( client, SIGNAL(newEntries(QList<Nepomuk::Query::Result>)),
             this, SLOT(addResults(QList<Nepomuk::Query::Result>)) );
    connect( client, SIGNAL(newEntries(QList<Nepomuk::Query::Result>)),
             this, SLOT(newEntries(QList<Nepomuk::Query::Result>)) );
    connect( client, SIGNAL(finishedListing()), client, SLOT(close()) );

    kDebug() << q.toSparqlQuery();
    client->query( q );
}


int PersonModel::rowCount(const QModelIndex& parent) const
{
    return m_people.size();
}

QModelIndex PersonModel::index(int row, int column, const QModelIndex& parent) const
{
    return QAbstractListModel::index(row, column, parent);
}

QVariant PersonModel::data(const QModelIndex& index, int role) const
{
    if( !index.isValid() )
        return QVariant();

    if( index.row() < 0 || index.row() >= m_people.size() )
        return QVariant();

    const Nepomuk::Person person = m_people.at( index.row() );
    switch( role ) {
        case PictureRole:
            return person.photo();

        case Qt::DisplayRole :
        case NameRole :
            if( !person.fullName().isEmpty() )
                return person.fullName();
            return person.nickName();

        case NicknameRole:
            return person.nickName();
    }

    return QVariant();
}

void PersonModel::addResults(const QList< Nepomuk::Query::Result >& results)
{
    beginInsertRows( QModelIndex(), m_people.size(), m_people.size() + results.size() );
    foreach( const Nepomuk::Query::Result &res, results ) {
        Nepomuk::Person person( res.resource().resourceUri() );
        if( !person.isEmpty() )
            m_people << person;
    }
    endInsertRows();
}
