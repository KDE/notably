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


#include "notewidget.h"
#include "noteedit.h"
#include "tageditor.h"

#include <QtGui/QVBoxLayout>

#include <Soprano/Model>
#include <Soprano/QueryResultIterator>

#include <Nepomuk/Variant>
#include <Nepomuk/ResourceManager>

#include <Nepomuk/Vocabulary/PIMO>
#include <Soprano/Vocabulary/NAO>

using namespace Nepomuk::Vocabulary;
using namespace Soprano::Vocabulary;

NoteWidget::NoteWidget(QWidget* parent, Qt::WindowFlags f): QWidget(parent, f)
{
    m_noteEditor = new NoteEdit( this );
    m_tagEditor = new TagEditor( this );

    QVBoxLayout *layout = new QVBoxLayout( this );
    layout->setMargin( 0 );
    layout->setSpacing( 0 );

    layout->addWidget( m_noteEditor );
    layout->addWidget( m_tagEditor );

    Nepomuk::Resource lastNote = lastUsedNote();
    m_noteEditor->setResource( lastNote );
    m_tagEditor->setTags( lastNote.tags() );
}

NoteWidget::~NoteWidget()
{
    saveNote();
}

void NoteWidget::newNote()
{
    m_noteEditor->save();
    m_noteEditor->reset();
    m_noteEditor->setFocus();
}

void NoteWidget::saveNote()
{
    m_noteEditor->save();
    Nepomuk::Resource noteResource = m_noteEditor->resource();
    noteResource.setTags( m_tagEditor->tags() );
}

Nepomuk::Resource NoteWidget::lastUsedNote() const
{
    // Show the last modified note
    QString query = QString::fromLatin1("select ?r where { ?r a %1. ?r %2 ?dt . } "
                                        "ORDER BY desc(?dt) LIMIT 1")
                    .arg( Soprano::Node::resourceToN3( PIMO::Note() ),
                          Soprano::Node::resourceToN3( NAO::lastModified() ) );

    Soprano::Model *model = Nepomuk::ResourceManager::instance()->mainModel();
    Soprano::QueryResultIterator it = model->executeQuery( query, Soprano::Query::QueryLanguageSparql );
    if( it.next() )
        return Nepomuk::Resource( it[0].uri() );
    else
        return Nepomuk::Resource();
}
