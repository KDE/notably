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


#include "mainwindow.h"
#include "personmodel.h"
#include "person.h"
#include "persondelegate.h"
#include "persontooltip.h"

#include <QtGui/QVBoxLayout>
#include <QtGui/QHBoxLayout>
#include <QtGui/QLabel>

#include <Nepomuk/Resource>
#include <Nepomuk/ResourceManager>
#include <Nepomuk/Variant>
#include <Nepomuk/Query/Query>
#include <Nepomuk/Query/QueryServiceClient>
#include <Nepomuk/Query/ResourceTerm>
#include <Nepomuk/Query/ResourceTypeTerm>
#include <Nepomuk/Query/ComparisonTerm>
#include <nepomuk/datamanagement.h>

#include <Soprano/Vocabulary/NAO>
#include <Nepomuk/Vocabulary/PIMO>

#include <KLocale>
#include <KJob>
#include <KDebug>
#include <KMessageBox>
#include <KRun>
#include <KIcon>
#include <KDialog>

using namespace Soprano::Vocabulary;
using namespace Nepomuk::Vocabulary;

MainWindow::MainWindow(QWidget* parent): QMainWindow(parent)
{
    QWidget *widget = new QWidget( this );

    m_mergeButton = new QPushButton(i18n("Merge People"), widget);
    connect( m_mergeButton, SIGNAL(clicked(bool)), this, SLOT(slotOnMerge()) );

    QHBoxLayout* hLayout = new QHBoxLayout;
    hLayout->addWidget( m_mergeButton );

    m_model = new PersonModel( widget );
    fillModel();

    m_filterModel = new QSortFilterProxyModel( this );
    m_filterModel->setSourceModel( m_model );
    m_filterModel->setFilterCaseSensitivity( Qt::CaseInsensitive );

    m_view = new QListView(widget);
    m_view->setModel( m_filterModel );
    m_view->setSelectionMode( QAbstractItemView::ExtendedSelection );
    m_view->setSelectionBehavior( QAbstractItemView::SelectRows );
    m_view->setItemDelegateForColumn( 0, new PersonDelegate(this) );
    m_view->setResizeMode( QListView::Adjust );
    m_view->setUniformItemSizes( false );
    m_view->setWordWrap( true );
    m_view->setHorizontalScrollBarPolicy( Qt::ScrollBarAlwaysOff );
    m_view->setAlternatingRowColors( true );

    QItemSelectionModel *selectionModel = m_view->selectionModel();
    connect( selectionModel, SIGNAL(selectionChanged(QItemSelection,QItemSelection)),
             this, SLOT(slotOnSelectionChange()) );
    slotOnSelectionChange();

    m_filterBar = new KLineEdit( this );
    connect( m_filterBar, SIGNAL(textChanged(QString)), this, SLOT(slotFilter(QString)) );

    QLabel* filterLabel = new QLabel(i18n("Filter: "), this);
    QHBoxLayout *filterLayout = new QHBoxLayout();
    filterLayout->addWidget( filterLabel );
    filterLayout->addWidget( m_filterBar );

    QVBoxLayout* layout = new QVBoxLayout();
    layout->addItem( filterLayout );
    layout->addWidget( m_view );
    layout->addItem( hLayout );

    m_tooltip = new PersonToolTip( this );

    QHBoxLayout* vLayout = new QHBoxLayout( widget );
    vLayout->addItem( layout );
    vLayout->addWidget( m_tooltip );

    setCentralWidget( widget );
    setWindowTitle( i18n("Nepomuk Person Manager") );
    setWindowIcon( KIcon("nepomuk") );
}

QList<QUrl> MainWindow::selectedPeople()
{
    QItemSelectionModel* selectedModel = m_view->selectionModel();
    if( !selectedModel->hasSelection() )
        return QList<QUrl>();

    QList<QUrl> list;
    QModelIndexList indexList = selectedModel->selectedIndexes();
    foreach(const QModelIndex& index, indexList) {
        const QUrl uri = index.data(PersonModel::UriRole).toUrl();
        list << uri;
    }

    return list;
}


void MainWindow::slotOnMerge()
{
    QList<QUrl> people = selectedPeople();
    if( people.size() < 2 )
        return;

    // Show confirmation message
    int answer = KMessageBox::questionYesNo( this, i18n("Do you want to merge these people?") );
    if( answer == KMessageBox::No )
        return;

    m_mergeList = people;

    const QUrl personUri = m_mergeList.at( 1 );
    m_mergeList.removeAt( 1 );
    kDebug() << "Merging " << personUri << m_mergeList.first();

    KJob *job = Nepomuk::mergeResources( m_mergeList.first(), personUri );
    connect( job, SIGNAL(finished(KJob*)), this, SLOT(slotOnMergeJob(KJob*)) );
}

void MainWindow::slotOnMergeJob(KJob* job)
{
    if( job->error() ) {
        kWarning() << job->errorString();
        KMessageBox::sorry(this, i18n("The people could not be merged"));
        return;
    }

    if( m_mergeList.size() == 1 ) {
        const QUrl uri = m_mergeList.first();
        m_mergeList.clear();

        Person p( uri );
        p.compress();

        KMessageBox::messageBox( this, KMessageBox::Information, i18n("People sucessfully merged") );
        fillModel();
        return;
    }

    const QUrl personUri = m_mergeList.at( 1 );
    m_mergeList.removeAt( 1 );
    kDebug() << "Merging " << personUri << m_mergeList.first();

    KJob *kjob = Nepomuk::mergeResources( m_mergeList.first(), personUri );
    connect( kjob, SIGNAL(finished(KJob*)), this, SLOT(slotOnMergeJob(KJob*)) );
}

void MainWindow::slotFilter(const QString& filter)
{
    m_filterModel->setFilterRegExp( filter );
}

void MainWindow::fillModel()
{
    Nepomuk::Query::ResourceTypeTerm term(PIMO::Person());
    Nepomuk::Query::Query q(term);

    m_model->setQuery( q );
}

void MainWindow::slotOnSelectionChange()
{
    QList<QUrl> people = selectedPeople();
    if( !people.isEmpty() )
        m_tooltip->setPerson( people.first() );
}
