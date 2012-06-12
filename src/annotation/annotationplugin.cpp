/*
 * This file is part of the Nepomuk KDE project.
 * Copyright (C) 2008 Adrien Bustany <madcat@mymadcat.com>
 * Copyright (c) 2008-2009 Sebastian Trueg <trueg@kde.org>
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Library General Public
 * License as published by the Free Software Foundation; either
 * version 2 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Library General Public License for more details.
 *
 * You should have received a copy of the GNU Library General Public License
 * along with this library; see the file COPYING.LIB.  If not, write to
 * the Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor,
 * Boston, MA 02110-1301, USA.
 */

#include "annotationplugin.h"
#include "annotationrequest.h"

class Nepomuk2::AnnotationPlugin::Private
{
public:
    Private( AnnotationPlugin* parent )
        : q( parent ) {
    }

    AnnotationRequest currentRequest;

private:
    AnnotationPlugin* q;
};



Nepomuk2::AnnotationPlugin::AnnotationPlugin(QObject* parent)
    : QObject(parent),
      d( new Private(this) )
{
}

Nepomuk2::AnnotationPlugin::~AnnotationPlugin()
{
    delete d;
}


void Nepomuk2::AnnotationPlugin::getPossibleAnnotations( const AnnotationRequest& request )
{
    d->currentRequest = request;
    doGetPossibleAnnotations( request );
}


void Nepomuk2::AnnotationPlugin::emitFinished()
{
    emit finished();
    emit finished( this );
}


void Nepomuk2::AnnotationPlugin::addNewAnnotation( Nepomuk2::Annotation* anno )
{
    emit newAnnotation( anno );
}


Nepomuk2::AnnotationRequest Nepomuk2::AnnotationPlugin::currentRequest() const
{
    return d->currentRequest;
}

#include "annotationplugin.moc"
