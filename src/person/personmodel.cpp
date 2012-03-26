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

    const Person person = m_people.at( index.row() );
    switch( role ) {
        case PictureRole:
        case Qt::DecorationRole:
            return person.photo();

        case Qt::DisplayRole :
        case NameRole :
            if( !person.fullName().isEmpty() )
                return person.fullName();
            return person.nickName();

        case NicknameRole:
            return person.nickName();

        case UriRole:
            return person.uri();
    }

    return QVariant();
}

void PersonModel::addResults(const QList< Nepomuk::Query::Result >& results)
{
    QList<Person> personList;
    foreach( const Nepomuk::Query::Result &res, results ) {

        Person person( res.resource().resourceUri() );
        if( !person.isEmpty() ) {
            personList << person;
            kDebug() << personList.size() + m_people.size() << "Loaded " << person.nickName();
        }
    }

    // the -1 is cause beginInsertRows takes the first and last row number
    beginInsertRows( QModelIndex(), m_people.size(), m_people.size() + personList.size()-1 );
    m_people << personList;
    endInsertRows();
}

void PersonModel::setQuery(Nepomuk::Query::Query& query)
{
    beginResetModel();
    m_people.clear();
    endResetModel();

    Nepomuk::Query::QueryServiceClient *client = new Nepomuk::Query::QueryServiceClient(this);
    connect( client, SIGNAL(newEntries(QList<Nepomuk::Query::Result>)),
             this, SLOT(addResults(QList<Nepomuk::Query::Result>)) );
    connect( client, SIGNAL(finishedListing()), client, SLOT(close()) );

    client->query( query );
}

void PersonModel::setList(const QList< Nepomuk::Resource >& people)
{
    beginResetModel();
    m_people.clear();
    endResetModel();

    QList<Person> personList;
    foreach( const Nepomuk::Resource& res, people ) {
        Person person( res.resourceUri() );
        if( !person.isEmpty() ) {
            personList << person;
            kDebug() << personList.size() << "Loaded " << person.nickName();
        }
    }

    beginInsertRows( QModelIndex(), 0, personList.size() );
    m_people = personList;
    endInsertRows();
}

QList< Nepomuk::Resource > PersonModel::toList()
{
    QList<Nepomuk::Resource> resList;
    foreach( const Person& person, m_people ) {
        resList << person.resource();
    }
    return resList;
}

