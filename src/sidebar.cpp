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


#include "sidebar.h"
#include "notebrowser.h"
#include "mainmenu.h"
#include "browsemenu.h"
#include "noteinformation.h"
#include "annotationgroupwidget.h"
#include "tags/tagcloud.h"

#include <QtCore/QCoreApplication>

#include <QtGui/QSortFilterProxyModel>
#include <QtGui/QBoxLayout>
#include <QtGui/QPushButton>
#include <QtGui/QStackedLayout>
#include <QtGui/QLabel>

#include <Soprano/Vocabulary/NAO>
#include <Nepomuk2/Vocabulary/NCO>

#include <Nepomuk2/Variant>

#include <KDebug>
#include <KIcon>

using namespace Soprano::Vocabulary;
using namespace Nepomuk2::Vocabulary;

Sidebar::Sidebar(QWidget* parent, Qt::WindowFlags f)
    : QWidget(parent, f)
{
    QWidget *navigationWidget = new QWidget();
    QHBoxLayout *hLayout = new QHBoxLayout( navigationWidget );
    hLayout->setMargin(0);
    hLayout->setSpacing(0);

    m_backButton = new QPushButton(KIcon("draw-arrow-back"), "");
    m_backButton->setFlat(true);
    connect( m_backButton, SIGNAL(clicked(bool)), this, SLOT(slotMoveBackward()) );

    m_forwardButton = new QPushButton(KIcon("draw-arrow-forward"), "");
    m_forwardButton->setFlat(true);
    connect( m_forwardButton, SIGNAL(clicked(bool)), this, SLOT(slotMoveForward()) );

    m_title = new QLabel(this);

    hLayout->addWidget( m_backButton, 0, Qt::AlignLeft );
    hLayout->addWidget( m_title, 100, Qt::AlignCenter );
    hLayout->addWidget( m_forwardButton, 0, Qt::AlignRight );

    m_mainMenu = new MainMenu( this );
    connect( m_mainMenu, SIGNAL(newNote()), this, SIGNAL(newNoteSelected()) );
    connect( m_mainMenu, SIGNAL(browseNotes()), this, SLOT(slotBrowseNotes()) );

    m_noteInfo = new NoteInformation( this );
    connect( m_noteInfo, SIGNAL(tagSelected(Nepomuk2::Tag)), this, SLOT(showTagInBrowser(Nepomuk2::Tag)) );
    connect( m_noteInfo, SIGNAL(personSelected(Nepomuk2::Resource)), this, SLOT(showPersonInBrowser(Nepomuk2::Resource)) );

    m_stackedLayout = new QStackedLayout();
    m_stackedLayout->setSpacing( 0 );
    m_stackedLayout->setMargin( 0 );
    push(QLatin1String("Main Menu"), m_mainMenu);
    //push(QLatin1String("Note Information"), m_noteInfo);

    m_title->setText(m_titleList[0]);

    QVBoxLayout * mainLayout = new QVBoxLayout( this );
    mainLayout->setSpacing(0);
    mainLayout->setMargin(0);
    mainLayout->addWidget( navigationWidget );
    mainLayout->addItem(m_stackedLayout);

    updateButtons();
}

Sidebar::~Sidebar()
{
}

void Sidebar::slotBrowseNotes()
{
    BrowseMenu* menu = new BrowseMenu(this);
    connect( menu, SIGNAL(browseByDateSelected()), this, SLOT(slotBrowseByDate()) );
    connect( menu, SIGNAL(browseByTagsSelected()), this, SLOT(slotBrowseByTags()) );

    push(i18n("Browse Notes"), menu);
    slotMoveForward();
}

void Sidebar::setTitle(const QString& title)
{
    m_title->setText(title);
}

void Sidebar::slotMoveBackward()
{
    int index = m_stackedLayout->currentIndex();
    if( index > 0 ) {
        index--;
        m_stackedLayout->setCurrentIndex(index);
        setTitle( m_titleList[index] );

        updateButtons();
    }
}

void Sidebar::slotMoveForward()
{
    int index = m_stackedLayout->currentIndex();
    if( index < m_stackedLayout->count()-1 ) {
        index++;
        m_stackedLayout->setCurrentIndex(index);
        setTitle( m_titleList[index] );
        updateButtons();
    }
}

void Sidebar::updateButtons()
{
    int index = m_stackedLayout->currentIndex();
    if( index == 0 ) {
        m_backButton->setDisabled( true );
        m_forwardButton->setDisabled( false );
    }
    else if( index == m_stackedLayout->count()-1 ) {
        m_backButton->setDisabled( false );
        m_forwardButton->setDisabled( true );
    }
    else {
        m_backButton->setDisabled( false );
        m_forwardButton->setDisabled( false );
    }
}

void Sidebar::push(const QString& title, QWidget* widget)
{
    int index = m_stackedLayout->currentIndex();

    // Remove all the widgets after index
    while( m_stackedLayout->count() != index+1 ) {
        int i = index+1;
        QWidget* widget = m_stackedLayout->widget(i);
        m_stackedLayout->removeWidget( widget );

        //WARNING: Special case for m_noteInfo
        if( widget != m_noteInfo )
            widget->deleteLater();

        m_titleList.removeAt(i);
    }
    Q_ASSERT( index < 0 || m_stackedLayout->count() == index+1 );

    m_stackedLayout->addWidget( widget );
    m_titleList.append( title );
}

void Sidebar::showInfo(const Nepomuk2::Resource& note)
{
    m_noteInfo->setNote( note );

    int current = m_stackedLayout->currentIndex();
    if( m_stackedLayout->widget(current) == m_noteInfo ) {
        return;
    }

    push(i18n("Note Information"), m_noteInfo);
    slotMoveForward();
}

bool Sidebar::saveNote(const Nepomuk2::Resource& note)
{
    return m_noteInfo->saveNote( note );
}

void Sidebar::newNote()
{
    m_noteInfo->newNote();
}

void Sidebar::showTagInBrowser(const Nepomuk2::Tag& tag)
{
    NoteBrowser* browser = new NoteBrowser( this );
    connect( browser, SIGNAL(noteSelected(Nepomuk2::Resource)),
             this, SIGNAL(noteSelected(Nepomuk2::Resource)) );
    browser->setTag( tag );
    browser->get();

    QString label = i18n("has Tag \"") + tag.genericLabel() + "\"";
    push( label, browser );

    // Maybe this should be done in push
    slotMoveForward();
}

void Sidebar::showPersonInBrowser(const Nepomuk2::Resource& person)
{
    NoteBrowser* browser = new NoteBrowser( this );
    connect( browser, SIGNAL(noteSelected(Nepomuk2::Resource)),
             this, SIGNAL(noteSelected(Nepomuk2::Resource)) );
    browser->setPerson( person );
    browser->get();

    QString label = i18n("Person \"") + person.property( NCO::nickname() ).toString() + "\"";
    // TODO: Maybe show something better than a label?
    push( label, browser );

    // Maybe this should be done in push
    slotMoveForward();
}

void Sidebar::slotBrowseByDate()
{
    NoteBrowser *browser = new NoteBrowser( this );
    connect( browser, SIGNAL(noteSelected(Nepomuk2::Resource)),
             this, SIGNAL(noteSelected(Nepomuk2::Resource)) );
    browser->get();

    QString label = i18n("Browse by date");
    push( label, browser );

    // Maybe this should be done in push
    slotMoveForward();
}

void Sidebar::slotBrowseByTags()
{
    TagCloud* cloud = new TagCloud( this );
    connect( cloud, SIGNAL(tagSelected(Nepomuk2::Tag)), this, SLOT(showTagInBrowser(Nepomuk2::Tag)) );

    push(i18n("Choose a tag"), cloud);
    slotMoveForward();
}

void Sidebar::showAnnotationGroup(TextAnnotationGroup* tag)
{
    AnnotationGroupWidget* widget = new AnnotationGroupWidget( tag, this );

    push(i18n("Choose an Annotation"), widget);
    slotMoveForward();
}
