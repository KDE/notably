/*
   Copyright (c) 2012 Vishesh Handa <me@vhanda.in>

   This library is free software; you can redistribute it and/or
   modify it under the terms of the GNU Lesser General Public
   License as published by the Free Software Foundation; either
   version 2.1 of the License, or (at your option) any later version.

   This library is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
   Lesser General Public License for more details.

   You should have received a copy of the GNU Lesser General Public License
   along with this library; see the file COPYING.LIB.  If not, write to
   the Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor,
   Boston, MA 02110-1301, USA.
*/

#ifndef _PERSON_ANNOTATION_PLUGIN_H_
#define _PERSON_ANNOTATION_PLUGIN_H_

#include "../../person/personmodel.h"

#include <Nepomuk/Resource>
#include <nepomuk/annotationplugin.h>

#include <QtGui/QSortFilterProxyModel>

class TextAnnotation;

class PersonAnnotationPlugin : public Nepomuk::AnnotationPlugin
{
    Q_OBJECT

public:
    PersonAnnotationPlugin( QObject* parent, const QVariantList& = QVariantList() );
    ~PersonAnnotationPlugin();

protected:
    void doGetPossibleAnnotations( const Nepomuk::AnnotationRequest& request );

private Q_SLOTS:

private:
    QModelIndexList matchingIndexes( const QString& text );
    QList<Person> matchingPeople( const QString& text );
    QSet<TextAnnotation*> createTextAnnotations(int group, const QString& word, int start, int len);
    void addAnnotations(const QSet<TextAnnotation*> annotations);

    PersonModel* m_personModel;
    QSortFilterProxyModel* m_filterModel;

};

#endif
