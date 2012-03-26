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

#include <Nepomuk/Vocabulary/NCO>
#include <Nepomuk/Vocabulary/PIMO>
#include <Nepomuk/Variant>
#include <KDebug>

using namespace Nepomuk::Vocabulary;

namespace {
    QFont boldFont(const QFont& f) {
        QFont font(f);
        font.setBold( true );
        return font;
    }
}

PersonToolTip::PersonToolTip(QWidget* parent, Qt::WindowFlags f)
    : QWidget(parent, f)
{
    setAttribute( Qt::WA_TranslucentBackground );
    setWindowFlags( Qt::ToolTip | Qt::FramelessWindowHint );

    m_preview = new QLabel( this );
    m_name = new QLabel( this );
    m_treeWidget = new QTreeWidget( this );

    m_name->setForegroundRole(QPalette::ToolTipText);
    m_name->setFont( boldFont(m_name->font()) );

    QVBoxLayout *vLayout = new QVBoxLayout();
    vLayout->addWidget( m_name, 0, Qt::AlignCenter );
    vLayout->addWidget( new KSeparator(this) );
    vLayout->addWidget( m_treeWidget, 0, Qt::AlignCenter );

    QHBoxLayout *hLayout = new QHBoxLayout( this );
    hLayout->addWidget( m_preview );
    hLayout->addSpacing( hLayout->margin() );
    hLayout->addLayout( vLayout );

    setLayout( hLayout );
    setPerson( Nepomuk::Resource() );
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

namespace {
    QPixmap personPixmap(const Person& person) {
        QPixmap pixmap;
        pixmap.load( person.photo().toLocalFile() );

        if( pixmap.isNull() ) {
            pixmap = KIcon("im-user").pixmap( QSize(96, 96) );
        }
        else {
            pixmap = pixmap.scaled( 96, 96, Qt::KeepAspectRatioByExpanding, Qt::SmoothTransformation );
        }

        return pixmap;
    }

    QIcon iconForType(const QString& type) {
        return KIcon( "im-" + type );
    }
}

void PersonToolTip::setPerson(const Nepomuk::Resource& resource)
{
    m_person = resource;

    Person person( m_person.resourceUri() );
    m_preview->setPixmap( personPixmap(person) );
    m_name->setText( person.displayName() );

    // Populate the QTreeWidget
    m_treeWidget->clear();
    m_treeWidget->setColumnCount( 1 );

    QList<QTreeWidgetItem*> contactItems;
    QList<Nepomuk::Resource> contactResources = person.personContacts();
    foreach( const Nepomuk::Resource& contact, contactResources ) {
        QTreeWidgetItem* item = new QTreeWidgetItem( 0 );

        QString displayName = contact.property( NCO::fullname() ).toString();
        if( displayName.isEmpty() )
            displayName = contact.property( NCO::nickname() ).toString();
        if( displayName.isEmpty() )
            displayName = i18n("Unnamed Contact");

        // Set the item's properties
        item->setIcon( 0, KIcon("im-user") );
        item->setText( 0, displayName );

        // Add all the IMAccounts
        QList<Nepomuk::Resource> accounts = contact.property( NCO::hasIMAccount() ).toResourceList();
        foreach( const Nepomuk::Resource& acc, accounts ) {
            const QString type = acc.property( NCO::imAccountType() ).toString();
            const QString id = acc.property( NCO::imID() ).toString();
            const QString nickname = acc.property( NCO::imNickname() ).toString();

            QTreeWidgetItem* accountItem = new QTreeWidgetItem( 0 );
            accountItem->setText( 0, QString("%1 (%2)").arg( nickname, id ) );
            accountItem->setIcon( 0, iconForType(type) );

            item->addChild( accountItem );
        }

        contactItems << item;
    }

    m_treeWidget->insertTopLevelItems( 0, contactItems );
    m_treeWidget->setHeaderHidden( true );
    m_treeWidget->setSelectionMode( QAbstractItemView::NoSelection );
    m_treeWidget->expandAll();

    m_treeWidget->setVerticalScrollBarPolicy( Qt::ScrollBarAlwaysOff );
    m_treeWidget->setHorizontalScrollBarPolicy( Qt::ScrollBarAlwaysOff );
    m_treeWidget->setSizePolicy( QSizePolicy::Minimum, QSizePolicy::Minimum );

    m_treeWidget->setAutoFillBackground( false );
    m_treeWidget->setAttribute( Qt::WA_TranslucentBackground, true );
}

Nepomuk::Resource PersonToolTip::person() const
{
    return m_person;
}
