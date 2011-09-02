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

NoteWidget::NoteWidget(QWidget* parent, Qt::WindowFlags f): QWidget(parent, f)
{
    m_noteEditor = new NoteEdit( this );
    m_tagEditor = new TagEditor( this );

    QVBoxLayout *layout = new QVBoxLayout( this );
    layout->setMargin( 0 );
    layout->setSpacing( 0 );

    layout->addWidget( m_noteEditor );
    layout->addWidget( m_tagEditor );
}

NoteWidget::~NoteWidget()
{
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
}
