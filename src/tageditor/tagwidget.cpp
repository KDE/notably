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


#include "tagwidget.h"
#include "tageditor.h"
#include "tagview.h"

#include <QtGui/QHBoxLayout>
#include <QtGui/QVBoxLayout>

#include <KPushButton>
#include <Nepomuk/Tag>

TagWidget::TagWidget(QWidget* parent, Qt::WindowFlags f): QWidget(parent, f)
{
    m_tagEditor = new TagEditor( this );
    KPushButton* addButton = new KPushButton("Add Tag", this);
    connect( addButton, SIGNAL(clicked(bool)), this, SLOT(slotAddTags()) );

    QHBoxLayout* hLayout = new QHBoxLayout();
    hLayout->addWidget( m_tagEditor, 0 );
    hLayout->addWidget( addButton, 0 );

    m_tagView = new TagView();
    connect( m_tagView, SIGNAL(tagClicked(Nepomuk::Tag)), this, SIGNAL(tagSelected(Nepomuk::Tag)) );

    QVBoxLayout* mainLayout = new QVBoxLayout( this );
    mainLayout->addItem( hLayout );
    mainLayout->addWidget( m_tagView );
}

void TagWidget::slotAddTags()
{
    QList< Nepomuk::Tag > tags = m_tagEditor->tags();
    addTags( tags );
    m_tagEditor->reset();
}

QList< Nepomuk::Tag > TagWidget::tags()
{
    return m_tagView->tags();
}

void TagWidget::setTags(const QList< Nepomuk::Tag >& tags)
{
    m_tagView->setTags( tags );
}

void TagWidget::addTags(const QList< Nepomuk::Tag >& tags)
{
    m_tagView->addTags( tags );
}

