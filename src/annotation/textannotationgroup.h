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


#ifndef TEXTANNOTATIONGROUP_H
#define TEXTANNOTATIONGROUP_H

#include "textannotation.h"
#include <QtCore/QObject>

class TextAnnotationGroup: public QObject
{
    Q_OBJECT
public:
    TextAnnotationGroup(const QList<TextAnnotation*>& annotations);
    explicit TextAnnotationGroup(TextAnnotation* ta);

    QList<TextAnnotation*> annotations();
    void setAnnotations(const QList<TextAnnotation*>& annotations);

    enum State {
        Accepted = 0,
        Rejected = 1,
        Listed
    };

    State state() const;
    void setState( const State& st );

signals:
    void stateChanged(const State& state);

private:
    QList<TextAnnotation*> m_annotations;
    State m_state;
};

Q_DECLARE_METATYPE(TextAnnotationGroup*);

#endif // TEXTANNOTATIONGROUP_H
