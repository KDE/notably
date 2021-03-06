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

#include <Nepomuk2/Query/QueryServiceClient>

#include <Nepomuk2/Types/Class>
#include <Nepomuk2/Variant>
#include <KDebug>


NotesModel::NotesModel(QObject* parent): SimpleResourceModel(parent)
{
}

void NotesModel::setQuery(Nepomuk2::Query::Query& query)
{
    clear();

    Nepomuk2::Query::QueryServiceClient *client = new Nepomuk2::Query::QueryServiceClient( this );
    connect( client, SIGNAL(newEntries(QList<Nepomuk2::Query::Result>)),
             this, SLOT(addResults(QList<Nepomuk2::Query::Result>)) );
    //FIXME: Ideally the model should be live updated, but using the QueryServiceClient for such
    //       things is quite expensive, as it runs the query every time the repository changes
    connect( client, SIGNAL(finishedListing()), client, SLOT(deleteLater()) );

    client->query( query );
}

NotesModel::~NotesModel()
{

}

QVariant NotesModel::data(const QModelIndex& index, int role) const
{
    if( !index.isValid() )
        return QVariant();

    Nepomuk2::Resource res = resourceForIndex( index );

    switch( role ) {
        case Nepomuk2::Utils::ResourceModel::ResourceRole: {
            return QVariant::fromValue( res );
        }
    }

    QUrl prop = propertyForRole( role );
    if( !prop.isEmpty() ) {
        return res.property( prop ).variant();
    }

    return QVariant();
}

QUrl NotesModel::propertyForRole(int role) const
{
    QHash< int, QUrl >::const_iterator it = m_rolePropertyHash.constFind( role );
    if( it != m_rolePropertyHash.constEnd() )
        return it.value();

    return QUrl();
}

int NotesModel::roleForProperty(const QUrl& property)
{
    QHash< QUrl, int >::const_iterator it = m_propertyRoleHash.constFind( property );
    if( it != m_propertyRoleHash.constEnd() )
        return it.value();

    // 5800 is a random number I've chosen
    int role = 5800 + m_propertyRoleHash.size();
    m_propertyRoleHash.insert( property, role );
    m_rolePropertyHash.insert( role, property );

    return role;
}

void NotesModel::emitDataUpdated(const Nepomuk2::Resource& res)
{
    QModelIndex index = indexForResource( res );
    emit dataChanged( index, index );
}
