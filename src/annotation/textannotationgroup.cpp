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


#include "textannotationgroup.h"

TextAnnotationGroup::TextAnnotationGroup( TextAnnotation* ta )
    : m_state( Accepted )
{
    m_annotations << ta;
}

TextAnnotationGroup::TextAnnotationGroup(const QList< TextAnnotation* >& annotations)
    : m_annotations( annotations )
{
    if( m_annotations.size() == 1 ) {
        m_state = Accepted;
    }
    else {
        m_state = Listed;
    }
}

QList< TextAnnotation* > TextAnnotationGroup::annotations()
{
    return m_annotations;
}

void TextAnnotationGroup::setAnnotations(const QList< TextAnnotation* >& annotations)
{
    m_annotations = annotations;
}

TextAnnotationGroup::State TextAnnotationGroup::state() const
{
    return m_state;
}

void TextAnnotationGroup::setState(const TextAnnotationGroup::State& st)
{
    if( m_state != st ) {
        m_state = st;
        emit stateChanged( st );
    }
}


