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


#include "annotator.h"

#include <KService>

Annotator::Annotator(QObject* parent)
: AnnotationPluginWrapper(parent)
{
//    FIXME: Get custom plugins for Notably
//    Nepomuk2::AnnotationPluginFactory *factory = Nepomuk2::AnnotationPluginFactory::instance();
    QList<Nepomuk2::AnnotationPlugin*> plugins;// = factory->getPluginsSupportingAnnotationOfType( PIMO::Note() );

    KService::Ptr ptr = KService::serviceByDesktopName("personannotationplugin");
    static Nepomuk2::AnnotationPlugin* personPlugin = ptr->createInstance<Nepomuk2::AnnotationPlugin>();
    plugins << personPlugin;

    setPlugins( plugins );
}

Annotator::~Annotator()
{

}

Annotator* Annotator::instance()
{
    //WARNING: We aren't supposed to create static QObject, cannot remember why
    static Annotator ann;
    return &ann;
}
