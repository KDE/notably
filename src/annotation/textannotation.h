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


#ifndef TEXTANNOTATION_H
#define TEXTANNOTATION_H

#include <nepomuk/annotation.h>

class TextAnnotation : public Nepomuk::Annotation
{
public:
    TextAnnotation(int start, int end, const QUrl& prop, const Nepomuk::Resource& obj, QObject* parent=0);

    int startPosition() const;
    int endPosition() const;

    QUrl property() const;
    Nepomuk::Resource object() const;

    bool exists(Nepomuk::Resource res) const;

    int group() const;
    void setGroup(int g);
protected:
    virtual void doCreate(Nepomuk::Resource res);

private:
    int m_startPos;
    int m_endPosition;

    const QUrl m_property;
    const Nepomuk::Resource m_object;

    int m_group;
};

#endif // TEXTANNOTATION_H
