/*
    <one line to give the library's name and an idea of what it does.>
    Copyright (C) 2011-12  Vishesh Handa <handa.vish@gmail.com>

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


#include "persontooltip.h"
#include "person.h"

#include <QtGui/QPixmap>
#include <QtGui/QVBoxLayout>
#include <QtGui/QHBoxLayout>
#include <QtGui/QStyleOptionFrame>
#include <QtGui/QStylePainter>

#include <KSeparator>
#include <KIcon>

PersonToolTip::PersonToolTip(QWidget* parent, Qt::WindowFlags f)
    : QWidget(parent, f)
{
    setAttribute( Qt::WA_TranslucentBackground );
    setWindowFlags( Qt::ToolTip | Qt::FramelessWindowHint );

    m_preview = new QLabel( this );
    m_name = new QLabel( this );
    m_name->setForegroundRole(QPalette::ToolTipText);

    // Make the font bold
    QFont font = m_name->font();
    font.setBold( true );
    m_name->setFont( font );

    QVBoxLayout *vLayout = new QVBoxLayout();
    vLayout->addWidget( m_name, 0, Qt::AlignCenter );
    vLayout->addWidget( new KSeparator(this) );
    //TODO: Add some kind of designation

    QHBoxLayout *hLayout = new QHBoxLayout( this );
    hLayout->addWidget( m_preview );
    hLayout->addSpacing( hLayout->margin() );
    hLayout->addLayout( vLayout );

    setLayout( hLayout );
}

PersonToolTip::~PersonToolTip()
{

}

void PersonToolTip::paintEvent(QPaintEvent* event)
{
    QStylePainter painter(this);
    QStyleOptionFrame option;
    option.initFrom(this);
    painter.drawPrimitive(QStyle::PE_PanelTipLabel, option);
    painter.end();

    QWidget::paintEvent(event);
}

void PersonToolTip::setPerson(const Nepomuk::Resource& resource)
{
    m_person = resource;

    Person person( m_person.resourceUri() );
    QPixmap pixmap;
    pixmap.load( person.photo().toLocalFile() );

    if( pixmap.isNull() ) {
        pixmap = KIcon("im-user").pixmap( QSize(32, 32) );
    }
    m_preview->setPixmap( pixmap );

    QString name = person.fullName();
    if( name.isEmpty() )
        name = person.nickName();
    m_name->setText( name );
}

Nepomuk::Resource PersonToolTip::person() const
{
    return m_person;
}
