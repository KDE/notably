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
#include "notesview.h"
#include "notesmodel.h"

#include <QtGui/QHBoxLayout>

Sidebar::Sidebar(QWidget* parent, Qt::WindowFlags f)
    : QWidget(parent, f)
{
    m_notesModel = new NotesModel( this );

    m_notesView = new NotesView( this );
    m_notesView->setModel( m_notesModel );

    connect( m_notesView, SIGNAL(doubleClicked(QModelIndex)),
             this, SLOT(slotNoteSelected(QModelIndex)) );

    //FIXME: Figure out why this stupid layout is required!
    QHBoxLayout* layout = new QHBoxLayout( this );
    layout->setMargin( 0 );
    layout->setSpacing( 0 );

    layout->addWidget( m_notesView );
    setLayout( layout );
}

Sidebar::~Sidebar()
{

}

void Sidebar::slotNoteSelected(const QModelIndex& index)
{
    Nepomuk::Resource res = m_notesModel->resourceForIndex(index);
    emit noteSelected( res );
}


