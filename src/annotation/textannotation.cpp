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

TextAnnotation::TextAnnotation(int start, int end, const QUrl& prop,
                               const Nepomuk::Resource& obj, QObject* parent)
    : Annotation(parent)
    , m_startPos( start )
    , m_endPosition( end )
    , m_property( prop )
    , m_object( obj )
    , m_group( -1 )
{

}

void TextAnnotation::doCreate(Nepomuk::Resource res)
{
}

bool TextAnnotation::exists(Nepomuk::Resource res) const
{
    return false;
}

int TextAnnotation::endPosition() const
{
    return m_endPosition;
}

int TextAnnotation::startPosition() const
{
    return m_startPos;
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
    return m_endPosition - m_startPos + 1;
}

void TextAnnotation::setEndPosition(int pos)
{
    m_endPosition = pos;
}

void TextAnnotation::setLength(int len)
{
    m_endPosition = m_startPos + len - 1;
}

void TextAnnotation::setStartPosition(int pos)
{
    m_startPos = pos;
}


