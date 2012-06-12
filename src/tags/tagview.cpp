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


#include "tagview.h"
#include "tag.h"

#include <QtGui/QMouseEvent>

TagView::TagView(QWidget* parent): QWidget(parent)
{
    m_layout = new FlowLayout( this );

    // Remove the background and border
    setAttribute( Qt::WA_NoSystemBackground );
    setAttribute( Qt::WA_NoBackground );
    setAutoFillBackground( false );
    setStyleSheet("background-color: transparent;");
}

void TagView::addTags(const QList< Nepomuk2::Tag >& tags)
{
    foreach( const Nepomuk2::Tag& t, tags ) {
        Tag *tagWidget = new Tag(this);
        tagWidget->setTag( t );
        connect( tagWidget, SIGNAL(tagSelected(Nepomuk2::Tag)), this, SIGNAL(tagClicked(Nepomuk2::Tag)) );
        connect( tagWidget, SIGNAL(tagDeleted(Nepomuk2::Tag)), this, SLOT(slotTagDeleted(Nepomuk2::Tag)) );

        m_tags.append( tagWidget );
        m_layout->addWidget( tagWidget );
    }
}

void TagView::setTags(const QList< Nepomuk2::Tag >& tags)
{
    clearTags();
    addTags( tags );
}

void TagView::clearTags()
{
    for( int i=0; i<m_tags.size(); i++ ) {
        QLayoutItem *item = m_layout->takeAt(0);
        if( item ) {
            QWidget *widget = item->widget();
            widget->deleteLater();
        }
    }
    m_tags.clear();
}

QList< Nepomuk2::Tag > TagView::tags()
{
    QList<Nepomuk2::Tag> tags;
    foreach( Tag* t, m_tags )
        tags << t->tag();
    return tags;
}

void TagView::slotTagDeleted(const Nepomuk2::Tag& tag)
{
    int index = -1;
    for(int i=0; i<m_tags.size(); i++) {
        Tag* tagWidget = m_tags[i];
        if( tagWidget->tag() == tag ) {
            index = i;
            break;
        }
    }

    if( index == -1 )
        return;

    Tag* tagWidget = m_tags[index];
    m_tags.removeAt( index );
    m_layout->takeAt( index );
    tagWidget->deleteLater();
}
