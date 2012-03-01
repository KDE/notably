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


#include "person.h"

#include <Nepomuk/Variant>
#include <Nepomuk/File>
#include <Nepomuk/Vocabulary/PIMO>
#include <Nepomuk/Vocabulary/NCO>

#include <KDebug>

using namespace Nepomuk::Vocabulary;

Person::Person(QObject* parent): QObject(parent)
{
}

Person::Person(const Person& p): QObject()
{
    m_fullName = p.m_fullName;
    m_nickName = p.m_nickName;
    m_photoUrl = p.m_photoUrl;
    m_pimoPerson = p.m_pimoPerson;
}

Person::Person(const QUrl& uri, QObject* parent): QObject(parent)
{
    m_pimoPerson = Nepomuk::Resource::fromResourceUri(uri);
    if( !m_pimoPerson.hasType(PIMO::Person()) ) {
        kWarning() << uri << "is not of type pimo::Person";
        return;
    }

    //FIXME: Check if the pimo:Person has been given a name, and photo

    QList<Nepomuk::Resource> occurances = m_pimoPerson.property(PIMO::groundingOccurrence()).toResourceList();
    QList<Nepomuk::Resource> contacts;
    foreach( const Nepomuk::Resource &res, occurances ) {
        if( res.hasType(NCO::PersonContact()) )
            contacts << res;
    }
    occurances.clear();

    setProperties(contacts);

    QList<Nepomuk::Resource> imAccounts;
    if( m_fullName.isEmpty() || m_nickName.isEmpty() || m_photoUrl.isEmpty() ) {
        foreach( const Nepomuk::Resource &con, contacts ) {
            imAccounts << con.property(NCO::hasIMAccount()).toResourceList();
        }
        setProperties(imAccounts);
    }
}

void Person::setProperties(const QList< Nepomuk::Resource >& resources)
{
    //kDebug() << resources.size();
    foreach( const Nepomuk::Resource &res, resources ) {
        if( m_fullName.isEmpty() ) {
            QString name = res.property(NCO::fullname()).toString();
            if( !name.isEmpty() )
                m_fullName = name;
        }

        if( m_nickName.isEmpty() ) {
            QString nick = res.property(NCO::nickname()).toString();
            if( !nick.isEmpty() ) {
                m_nickName = nick;
            }

            nick = res.property(NCO::imNickname()).toString();
            if( !nick.isEmpty() ) {
                m_nickName = nick;
            }
        }

        if( m_photoUrl.isEmpty() ) {
            QUrl url = res.property(NCO::photo()).toResource().toFile().url();
            if( !url.isEmpty() )
                m_photoUrl = url;
        }
    }
}

QString Person::fullName() const
{
    return m_fullName;
}

QString Person::nickName() const
{
    return m_nickName;
}

QUrl Person::photo() const
{
    return m_photoUrl;
}

QUrl Person::uri() const
{
    return m_pimoPerson.resourceUri();
}

Nepomuk::Resource Person::resource() const
{
    return m_pimoPerson;
}

bool Person::isEmpty() const
{
    return m_fullName.isEmpty() && m_nickName.isEmpty() && m_photoUrl.isEmpty();
}
