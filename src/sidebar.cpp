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


#include "sidebar.h"
#include "notebrowser.h"

#include <QtGui/QSortFilterProxyModel>
#include <QtGui/QHBoxLayout>

#include <Soprano/Vocabulary/NAO>
#include <KDebug>

using namespace Soprano::Vocabulary;

Sidebar::Sidebar(QWidget* parent, Qt::WindowFlags f)
    : QWidget(parent, f)
{
    m_noteBrowser = new NoteBrowser( this );
    connect( m_noteBrowser, SIGNAL(noteSelected(Nepomuk::Resource)),
             this, SIGNAL(noteSelected(Nepomuk::Resource)) );

    //FIXME: Figure out why this stupid layout is required!
    QHBoxLayout* layout = new QHBoxLayout( this );
    layout->setMargin( 0 );
    layout->setSpacing( 0 );

    layout->addWidget( m_noteBrowser );
}

Sidebar::~Sidebar()
{
}

void Sidebar::noteSaved(const Nepomuk::Resource& note)
{
//     // If it already exists, then just update the view
//     for( int i=0; i<m_notesModel->rowCount(); i++ ) {
//         QModelIndex index = m_notesModel->index( i, 0 );
//         Nepomuk::Resource res = m_notesModel->resourceForIndex( index );
//
//         if( note == res ) {
//             m_notesModel->emitDataUpdated( note );
//             return;
//         }
//     }

    /*
    // TODO: Find a a better way!
    //       sorting the model again is not an option, neither is re-creating the entire model
    // Otherwise add the note
    m_notesModel->addResource( note );
    m_sortingModel->sort( 0, Qt::DescendingOrder );
    */

//     m_notesModel->reset();
}
