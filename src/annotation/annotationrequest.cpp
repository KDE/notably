/*
 * This file is part of the Nepomuk KDE project.
 * Copyright (c) 2009-2011 Sebastian Trueg <trueg@kde.org>
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

#include "annotationrequest.h"
#include "annotationrequest_p.h"

Nepomuk2::AnnotationRequest::AnnotationRequest( const Resource& resource )
    : d( new AnnotationRequestPrivate() )
{
    d->resource = resource;
}


Nepomuk2::AnnotationRequest::AnnotationRequest( const AnnotationRequest& other )
{
    d = other.d;
}


Nepomuk2::AnnotationRequest::~AnnotationRequest()
{
}


Nepomuk2::AnnotationRequest& Nepomuk2::AnnotationRequest::operator=( const AnnotationRequest& other )
{
    d = other.d;
    return *this;
}


Nepomuk2::Resource Nepomuk2::AnnotationRequest::resource() const
{
    return d->resource;
}


QString Nepomuk2::AnnotationRequest::filter() const
{
    return d->filter;
}


void Nepomuk2::AnnotationRequest::setResource( const Resource& res )
{
    d->resource = res;
}


void Nepomuk2::AnnotationRequest::setFilter( const QString& filter )
{
    d->filter = filter;
}


// static
Nepomuk2::AnnotationRequest Nepomuk2::AnnotationRequest::createSimpleFilterRequest( const Nepomuk2::Resource& resource, const QString& filter )
{
    AnnotationRequest request( resource );
    request.setFilter( filter );
    return request;
}
