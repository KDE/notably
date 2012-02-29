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

#include <QtCore/QObject>
#include <QtCore/QUrl>

#include <Nepomuk/Resource>

class Person : public QObject
{
    Q_OBJECT
public:
    explicit Person(QObject* parent = 0);
    Person(const Person& p);

    // Takes the pimo:Person uri
    Person(const QUrl& uri, QObject* parent = 0);

    QString fullName() const;
    QString nickName() const;
    QUrl photo() const;
    QUrl uri() const;

    bool isEmpty() const;
private:
    Nepomuk::Resource m_pimoPerson;

    QString m_fullName;
    QString m_nickName;
    QUrl m_photoUrl;

    void setProperties(const QList<Nepomuk::Resource> &resources);
};

#endif // PERSON_H
