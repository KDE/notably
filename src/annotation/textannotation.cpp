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


#include "textannotation.h"

TextAnnotation::TextAnnotation(int pos, int len, const QUrl& prop,
                               const Nepomuk::Resource& obj, QObject* parent)
    : Annotation(parent)
    , m_position( pos )
    , m_length( len )
    , m_property( prop )
    , m_object( obj )
    , m_group( -1 )
{

}

void TextAnnotation::doCreate(Nepomuk::Resource)
{
}

bool TextAnnotation::exists(Nepomuk::Resource) const
{
    return false;
}

int TextAnnotation::position() const
{
    return m_position;
}

Nepomuk::Resource TextAnnotation::object() const
{
    return m_object;
}

QUrl TextAnnotation::property() const
{
    return m_property;
}

int TextAnnotation::group() const
{
    return m_group;
}

void TextAnnotation::setGroup(int g)
{
    m_group = g;
}

int TextAnnotation::length() const
{
    return m_length;
}

void TextAnnotation::setLength(int len)
{
    m_length = len;
}

void TextAnnotation::setPosition(int pos)
{
    m_position = pos;
}

void TextAnnotation::setText(const QString& string)
{
    m_text = string;
}

QString TextAnnotation::text() const
{
    return m_text;
}
