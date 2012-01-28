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


#include "noteinformation.h"

#include <QtGui/QVBoxLayout>
#include <QtGui/QHBoxLayout>
#include <QtGui/QBoxLayout>

#include <Soprano/Vocabulary/NAO>
#include <Nepomuk/Vocabulary/NIE>

#include <Nepomuk/Variant>

using namespace Soprano::Vocabulary;
using namespace Nepomuk::Vocabulary;

NoteInformation::NoteInformation(QWidget* parent, Qt::WindowFlags f): QWidget(parent, f)
{
    QVBoxLayout* mainLayout = new QVBoxLayout( this );

    m_titleEdit = new KLineEdit();
    m_titleEdit->setPlaceholderText(i18n("Set note title"));

    m_createdLabel = new QLabel();
    m_modifiedLabel = new QLabel();

    mainLayout->addWidget( m_titleEdit );
    mainLayout->addWidget( m_createdLabel );
    mainLayout->addWidget( m_modifiedLabel );

    m_tagEditor = new TagEditor;
    mainLayout->addWidget( m_tagEditor );
}

void NoteInformation::setNote(const Nepomuk::Resource& note)
{
    if( m_note != note ) {
        m_note = note;
        updateView();
    }
}

void NoteInformation::updateView()
{
    QString modified = m_note.property(NAO::lastModified()).toDateTime().toString();
    QString created = m_note.property(NAO::created()).toDateTime().toString();

    m_modifiedLabel->setText( modified );
    m_createdLabel->setText( created );

    m_tagEditor->setTags( m_note.tags() );

    QString title = m_note.property(NIE::title()).toString();
    m_titleEdit->setText(title);
}

bool NoteInformation::saveNote(const Nepomuk::Resource& note)
{
    if( note != m_note )
        m_note = note;

    bool saved = false;
    QList<Nepomuk::Tag> newTags = m_tagEditor->tags();
    if( newTags !=  m_note.tags() ) {
        m_note.setTags( newTags );
        saved = true;
    }

    QString title = m_note.property(NIE::title()).toString();
    if( m_titleEdit->text() != title ) {
        m_note.setProperty(NIE::title(), m_titleEdit->text());
    }
    return saved;
}

void NoteInformation::newNote()
{
    m_note = Nepomuk::Resource();
    m_titleEdit->clear();
}
