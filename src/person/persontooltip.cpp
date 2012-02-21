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


#include "persontooltip.h"

#include <QtGui/QPixmap>
#include <QtGui/QVBoxLayout>
#include <QtGui/QHBoxLayout>
#include <QtGui/QStyleOptionFrame>
#include <QtGui/QStylePainter>

#include <KSeparator>

PersonToolTip::PersonToolTip(const Nepomuk::Resource& resource, QWidget* parent, Qt::WindowFlags f)
    : QWidget(parent, f)
{
    setAttribute( Qt::WA_TranslucentBackground );
    setWindowFlags( Qt::ToolTip | Qt::FramelessWindowHint );

    // Set the nco:photo
    // FIXME: Load from Nepomuk
    QPixmap pixmap;
    //FIXME: Resize to a particular size. 50x50 looks nice.
    pixmap.load("/home/vishesh/Images/avatars/ef98397f575d96e31ef5973abc67f97d7b3bbd0e");

    m_preview = new QLabel( this );
    m_preview->setPixmap( pixmap );

    QLabel *name = new QLabel( this );
    name->setText( QLatin1String("Test Person") );
    name->setForegroundRole(QPalette::ToolTipText);

    // Make the font bold
    QFont font = name->font();
    font.setBold( true );
    name->setFont( font );

    QVBoxLayout *vLayout = new QVBoxLayout();
    vLayout->addWidget( name, 0, Qt::AlignCenter );
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
