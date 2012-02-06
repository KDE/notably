/*
    <one line to give the library's name and an idea of what it does.>
    Copyright (C) 2011  Vishesh Handa <handa.vish@gmail.com>

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


#include "notebrowser.h"
#include "notesview.h"
#include "notesmodel.h"

#include <QtGui/QHBoxLayout>
#include <QtGui/QSortFilterProxyModel>

#include <Nepomuk/Query/Query>
#include <Nepomuk/Query/ResourceTypeTerm>
#include <Nepomuk/Query/ResourceTerm>
#include <Nepomuk/Query/ComparisonTerm>

#include <Nepomuk/Vocabulary/PIMO>
#include <Soprano/Vocabulary/NAO>

#include <KDebug>

using namespace Nepomuk::Vocabulary;
using namespace Soprano::Vocabulary;

NoteBrowser::NoteBrowser(QWidget* parent, Qt::WindowFlags f): QWidget(parent, f)
{
    m_model = new NotesModel( this );

    QSortFilterProxyModel * sortingModel = new QSortFilterProxyModel( this );
    sortingModel->setSourceModel( m_model );
    sortingModel->setSortRole( m_model->roleForProperty( NAO::created() ) );
    sortingModel->setDynamicSortFilter( true );
    sortingModel->sort( 0, Qt::DescendingOrder );

    m_view = new NotesView( this );
    m_view->setModel( sortingModel );

    connect( m_view, SIGNAL(doubleClicked(QModelIndex)),
             this, SLOT(slotNoteSelected(QModelIndex)) );

    Nepomuk::Query::ResourceTypeTerm typeTerm( Nepomuk::Types::Class( PIMO::Note() ) );
    Nepomuk::Query::ComparisonTerm compTerm( NAO::created(), Nepomuk::Query::Term() );
    compTerm.setSortWeight( 1, Qt::DescendingOrder );

    m_query = typeTerm && compTerm;

    //FIXME: Figure out why this stupid layout is required!
    QHBoxLayout* layout = new QHBoxLayout( this );
    layout->setMargin( 0 );
    layout->setSpacing( 0 );

    layout->addWidget( m_view );
}

NoteBrowser::~NoteBrowser()
{
}

void NoteBrowser::slotNoteSelected(const QModelIndex& index)
{
    Nepomuk::Resource res = m_model->resourceForIndex(index);
    emit noteSelected( res );
}

void NoteBrowser::setTag(const Nepomuk::Tag& tag)
{
    m_query = m_query && Nepomuk::Query::ComparisonTerm(NAO::hasTag(),
                                                        Nepomuk::Query::ResourceTerm(tag));
}

void NoteBrowser::get()
{
    m_model->setQuery( m_query );
}


