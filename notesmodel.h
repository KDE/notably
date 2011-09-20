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


#ifndef NOTESMODEL_H
#define NOTESMODEL_H

#include <QtCore/QHash>
#include <Nepomuk/Utils/SimpleResourceModel>

class NotesModel : public Nepomuk::Utils::SimpleResourceModel
{
public:
    NotesModel(QObject* parent = 0);
    virtual ~NotesModel();

    QVariant data(const QModelIndex& index, int role = Qt::DisplayRole) const;

    QUrl propertyForRole( int role ) const;
    int roleForProperty( const QUrl& property );

private:
    QHash<QUrl, int> m_propertyRoleHash;
    QHash<int, QUrl> m_rolePropertyHash;
};

#endif // NOTESMODEL_H
