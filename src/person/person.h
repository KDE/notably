/*
    <one line to give the library's name and an idea of what it does.>
    Copyright (C) 2012  Vishesh Handa <handa.vish@gmail.com>

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


#ifndef PERSON_H
#define PERSON_H

#include <QtCore/QUrl>

#include <Nepomuk2/Resource>

class Person
{
public:
    Person();
    Person(const Person& p);

    // Takes the pimo:Person uri
    Person(const QUrl& uri);

    QString fullName() const;
    QString nickName() const;
    QString displayName() const;
    QUrl photo() const;
    QUrl uri() const;
    Nepomuk2::Resource resource() const;

    bool isEmpty() const;

    void compress();

    QList<Nepomuk2::Resource> personContacts() const;
private:
    Nepomuk2::Resource m_pimoPerson;

    QString m_fullName;
    QString m_nickName;
    QUrl m_photoUrl;

    void setProperties(const QList<Nepomuk2::Resource> &resources);

    bool canMergeContacts(const Nepomuk2::Resource& c1, const Nepomuk2::Resource& c2);
};

#endif // PERSON_H
