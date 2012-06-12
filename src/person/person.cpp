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

#include <Nepomuk2/Variant>
#include <Nepomuk2/File>
#include <Nepomuk2/Vocabulary/PIMO>
#include <Nepomuk2/Vocabulary/NCO>

#include <Nepomuk2/DataManagement>

#include <KDebug>
#include <KJob>

using namespace Nepomuk2::Vocabulary;

Person::Person()
{
}

Person::Person(const Person& p)
{
    m_fullName = p.m_fullName;
    m_nickName = p.m_nickName;
    m_photoUrl = p.m_photoUrl;
    m_pimoPerson = p.m_pimoPerson;
}

Person::Person(const QUrl& uri)
{
    m_pimoPerson = Nepomuk2::Resource::fromResourceUri(uri);
    if( !m_pimoPerson.hasType(PIMO::Person()) ) {
        kWarning() << uri << "is not of type pimo::Person";
        return;
    }

    //FIXME: Check if the pimo:Person has been given a name, and photo

    QList<Nepomuk2::Resource> contacts = personContacts();
    setProperties(contacts);

    QList<Nepomuk2::Resource> imAccounts;
    if( m_fullName.isEmpty() || m_nickName.isEmpty() || m_photoUrl.isEmpty() ) {
        foreach( const Nepomuk2::Resource &con, contacts ) {
            imAccounts << con.property(NCO::hasIMAccount()).toResourceList();
        }
        setProperties(imAccounts);
    }
}

void Person::setProperties(const QList< Nepomuk2::Resource >& resources)
{
    //kDebug() << resources.size();
    foreach( const Nepomuk2::Resource &res, resources ) {
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

QString Person::displayName() const
{
    if( m_fullName.isEmpty() )
        return m_nickName;
    else
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

Nepomuk2::Resource Person::resource() const
{
    return m_pimoPerson;
}

bool Person::isEmpty() const
{
    return m_fullName.isEmpty() && m_nickName.isEmpty() && m_photoUrl.isEmpty();
}

QList< Nepomuk2::Resource > Person::personContacts() const
{
    QList<Nepomuk2::Resource> occurances = m_pimoPerson.property(PIMO::groundingOccurrence()).toResourceList();
    QList<Nepomuk2::Resource> contacts;
    foreach( const Nepomuk2::Resource &res, occurances ) {
        if( res.hasType(NCO::PersonContact()) )
            contacts << res;
    }

    return contacts;
}

void Person::compress()
{
    QList<Nepomuk2::Resource> contacts = personContacts();

    Nepomuk2::Resource lastMergedContact;
    QListIterator<Nepomuk2::Resource> iter( contacts );
    while( iter.hasNext() ) {
        Nepomuk2::Resource pc = iter.next();
        if( !lastMergedContact.isValid() ) {
            lastMergedContact = pc;
            continue;
        }

        if( canMergeContacts(lastMergedContact, pc) ) {
            const QUrl uri1 = lastMergedContact.resourceUri();
            const QUrl uri2 = pc.resourceUri();

            KJob* job = Nepomuk2::mergeResources( uri1, uri2 );
            job->exec();
            if( job->error() ) {
                kWarning() << job->errorString();
                return;
            }

            kDebug() << "Merged " << uri1 << uri2;
        }
    }

    // The cache should now be cleared
}

bool Person::canMergeContacts(const Nepomuk2::Resource& c1, const Nepomuk2::Resource& c2)
{
    if( !c1.hasType(NCO::PersonContact()) || !c2.hasType(NCO::PersonContact()) )
        return false;

    const QString c1Name = c1.property( NCO::fullname() ).toString();
    const QString c2Name = c2.property( NCO::fullname() ).toString();

    if( !c1Name.isEmpty() && !c2Name.isEmpty() )
        if( c1Name != c2Name )
            return false;

    kDebug() << c1Name << c2Name;

    const QString c1Nickname = c1.property( NCO::nickname() ).toString();
    const QString c2Nickname = c2.property( NCO::nickname() ).toString();

    if( !c1Nickname.isEmpty() && !c2Nickname.isEmpty() )
        if( c1Nickname != c2Nickname )
            return false;

    kDebug() << c1Nickname << c2Nickname;

    const Nepomuk2::Resource c1Gender = c1.property( NCO::gender() ).toResource();
    const Nepomuk2::Resource c2Gender = c2.property( NCO::gender() ).toResource();

    if( !c1Gender.isValid() && !c2Gender.isValid() )
        if( c1Gender != c2Gender )
            return false;

    kDebug() << c1Gender.genericLabel() << c2Gender.genericLabel();

    return true;
}
