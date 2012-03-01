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


#include "personcompleter.h"
#include "personmodel.h"
#include "persondelegate.h"

#include <QtGui/QListView>

#include <Nepomuk/Query/Query>
#include <Nepomuk/Query/ResourceTypeTerm>

#include <Nepomuk/Vocabulary/PIMO>

using namespace Nepomuk::Vocabulary;

PersonCompleter::PersonCompleter(QObject* parent): QCompleter(parent)
{
    Nepomuk::Query::ResourceTypeTerm term(PIMO::Person());
    Nepomuk::Query::Query q(term);

    PersonModel* model = new PersonModel( this );
    model->setQuery( q );

    QListView* view = new QListView();
    view->setItemDelegateForColumn( 0, new PersonDelegate(this) );
    view->setResizeMode( QListView::Adjust );
    view->setUniformItemSizes( false );

    setModel( model );
    setCompletionRole( Qt::DisplayRole );
    setPopup( view );
    setCaseSensitivity( Qt::CaseInsensitive );
}

PersonCompleter::~PersonCompleter()
{

}
