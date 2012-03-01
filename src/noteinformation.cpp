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
#include "tags/tagwidget.h"
#include "person/persongrid.h"

#include <QtGui/QVBoxLayout>
#include <QtGui/QHBoxLayout>
#include <QtGui/QBoxLayout>
#include <QtGui/QGroupBox>

#include <Soprano/Vocabulary/NAO>
#include <Nepomuk/Vocabulary/NIE>

#include <Nepomuk/Variant>
#include <KDebug>

using namespace Soprano::Vocabulary;
using namespace Nepomuk::Vocabulary;

namespace {
    class Row: public QWidget {
    public:
        Row(const QString& labelString, QWidget* other, QWidget* parent)
        : QWidget(parent)
        {
            QHBoxLayout* layout = new QHBoxLayout(this);
            QLabel* label = new QLabel(labelString);
            layout->addStretch(100);
            layout->addWidget(label);
            layout->addWidget(other);
        }
    };
}

NoteInformation::NoteInformation(QWidget* parent, Qt::WindowFlags f): QWidget(parent, f)
{
    QWidget *widget = new QWidget( this );
    QVBoxLayout *wLayout = new QVBoxLayout(widget);

    m_titleEdit = new KLineEdit();
    m_titleEdit->setAlignment(Qt::AlignRight);
    m_titleEdit->setPlaceholderText(i18n("Set note title"));

    m_createdLabel = new QLabel();
    m_modifiedLabel = new QLabel();

    wLayout->addWidget( m_titleEdit );
    wLayout->addWidget( new Row("<b>Created:</b>", m_createdLabel, this) );
    wLayout->addWidget( new Row("<b>Modified:</b>", m_modifiedLabel, this) );

    QGroupBox* tagBox = new QGroupBox( i18n("Tags"), this );
    QVBoxLayout* tagLayout = new QVBoxLayout( tagBox );
    m_tagWidget = new TagWidget( tagBox );
    tagLayout->addWidget( m_tagWidget );
    connect( m_tagWidget, SIGNAL(tagSelected(Nepomuk::Tag)), this, SIGNAL(tagSelected(Nepomuk::Tag)) );

    QGroupBox* personBox = new QGroupBox( i18n("People"), this );
    QVBoxLayout* personLayout = new QVBoxLayout( personBox );
    m_personGrid = new PersonGrid( personBox );
    personLayout->addWidget( m_personGrid );

    QVBoxLayout* mainLayout = new QVBoxLayout( this );
    mainLayout->addWidget( widget, 0, Qt::AlignTop );
    mainLayout->addWidget( personBox, 0, Qt::AlignBottom );
    mainLayout->addWidget( tagBox, 0, Qt::AlignBottom );
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
    QDateTime modified = m_note.property(NAO::lastModified()).toDateTime().toLocalTime();
    QDateTime created = m_note.property(NAO::created()).toDateTime().toLocalTime();

    QString modifiedString;
    QString createdString;

    if( modified.isValid() )
        modifiedString = KGlobal::locale()->formatDateTime(modified);
    if( created.isValid() )
        createdString = KGlobal::locale()->formatDateTime(created);

    m_modifiedLabel->setText( modifiedString );
    m_createdLabel->setText( createdString );

    m_tagWidget->setTags( m_note.tags() );
    // FIXME: Other types of resources could also be related!
    m_personGrid->setPeople( m_note.property( NAO::isRelated() ).toResourceList() );

    QString title = m_note.property(NIE::title()).toString();
    m_titleEdit->setText(title);
}

bool NoteInformation::saveNote(const Nepomuk::Resource& note)
{
    if( note != m_note )
        m_note = note;

    bool saved = false;
    QList<Nepomuk::Tag> newTags = m_tagWidget->tags();
    if( newTags !=  m_note.tags() ) {
        m_note.setTags( newTags );
        saved = true;
    }

    QString title = m_note.property(NIE::title()).toString();
    if( m_titleEdit->text() != title ) {
        m_note.setProperty(NIE::title(), m_titleEdit->text());
    }

    updateView();
    return saved;
}

void NoteInformation::newNote()
{
    m_note = Nepomuk::Resource();
    updateView();
}
