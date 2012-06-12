/*
   Copyright (c) 2008 Sebastian Trueg <trueg@kde.org>

   This library is free software; you can redistribute it and/or
   modify it under the terms of the GNU Library General Public
   License version 2 as published by the Free Software Foundation.

   This library is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
   Library General Public License for more details.

   You should have received a copy of the GNU Library General Public License
   along with this library; see the file COPYING.LIB.  If not, write to
   the Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor,
   Boston, MA 02110-1301, USA.
*/

#include "annotationpluginwrapper.h"
#include "annotationplugin.h"
#include "annotation.h"

#include "kdebug.h"

class Nepomuk2::AnnotationPluginWrapper::Private
{
public:
    QList<AnnotationPlugin*> plugins;
    QList<AnnotationPlugin*> runningPlugins;

    void _k_slotNewAnnotation( Nepomuk2::Annotation* );
    void _k_slotPluginFinished( Nepomuk2::AnnotationPlugin* );

    AnnotationPluginWrapper* q;
};


void Nepomuk2::AnnotationPluginWrapper::Private::_k_slotNewAnnotation( Nepomuk2::Annotation* anno )
{
    kDebug() << anno->label() << anno->relevance();
    emit q->newAnnotation( anno );
}


void Nepomuk2::AnnotationPluginWrapper::Private::_k_slotPluginFinished( Nepomuk2::AnnotationPlugin* plugin )
{
    runningPlugins.removeAll( plugin );
    if ( runningPlugins.isEmpty() ) {
        emit q->finished();
    }
}



Nepomuk2::AnnotationPluginWrapper::AnnotationPluginWrapper( QObject* parent )
    : QObject( parent ),
      d( new Private() )
{
    d->q = this;
}


Nepomuk2::AnnotationPluginWrapper::~AnnotationPluginWrapper()
{
    delete d;
}


void Nepomuk2::AnnotationPluginWrapper::setPlugins( const QList<AnnotationPlugin*>& plugins )
{
    qDeleteAll( d->plugins );

    d->plugins = plugins;

    foreach( Nepomuk2::AnnotationPlugin* plugin, d->plugins ) {
        connect( plugin, SIGNAL( newAnnotation( Nepomuk2::Annotation* ) ),
                 this, SLOT( _k_slotNewAnnotation( Nepomuk2::Annotation* ) ) );
        connect( plugin, SIGNAL( finished( Nepomuk2::AnnotationPlugin* ) ),
                 this, SLOT( _k_slotPluginFinished( Nepomuk2::AnnotationPlugin* ) ) );
    }
}


void Nepomuk2::AnnotationPluginWrapper::getPossibleAnnotations( const Nepomuk2::AnnotationRequest& request )
{
    d->runningPlugins = d->plugins;
    foreach( Nepomuk2::AnnotationPlugin* plugin, d->plugins ) {
        plugin->getPossibleAnnotations( request );
    }
}

#include "annotationpluginwrapper.moc"
