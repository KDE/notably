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

#include <Nepomuk/Query/QueryServiceClient>

#include <Nepomuk/Types/Class>
#include <Nepomuk/Variant>
#include <KDebug>


NotesModel::NotesModel(QObject* parent): SimpleResourceModel(parent)
{
}

void NotesModel::setQuery(Nepomuk::Query::Query& query)
{
    clear();

    Nepomuk::Query::QueryServiceClient *client = new Nepomuk::Query::QueryServiceClient( this );
    connect( client, SIGNAL(newEntries(QList<Nepomuk::Query::Result>)),
             this, SLOT(addResults(QList<Nepomuk::Query::Result>)) );
    //connect( client, SIGNAL(newEntries(QList<Nepomuk::Query::Result>)),
    //         this, SLOT(newEntries(QList<Nepomuk::Query::Result>)) );
    //connect( client, SIGNAL(finishedListing()), client, SLOT(deleteLater()) );

    client->query( query );
}

NotesModel::~NotesModel()
{

}

QVariant NotesModel::data(const QModelIndex& index, int role) const
{
    if( !index.isValid() )
        return QVariant();

    Nepomuk::Resource res = resourceForIndex( index );

    switch( role ) {
        case Nepomuk::Utils::ResourceModel::ResourceRole: {
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

void NotesModel::emitDataUpdated(const Nepomuk::Resource& res)
{
    QModelIndex index = indexForResource( res );
    emit dataChanged( index, index );
}

void NotesModel::newEntries(const QList< Nepomuk::Query::Result > & results)
{
    kDebug() << "----------";
    foreach( const Nepomuk::Query::Result & res, results ) {
        kDebug() << res.resource();
    }
}
