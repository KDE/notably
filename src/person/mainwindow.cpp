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

    QPushButton* deleteButton = new QPushButton(i18n("Delete Tags"), widget);
    m_renameMergeButton = new QPushButton(widget);

    QHBoxLayout* hLayout = new QHBoxLayout;
    hLayout->addWidget( m_renameMergeButton );
    hLayout->addWidget( deleteButton );

    m_model = new PersonModel( widget );

    Nepomuk::Query::ResourceTypeTerm term(PIMO::Person());
    Nepomuk::Query::Query q(term);
    m_model->setQuery( q );

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
    //connect( m_view, SIGNAL(doubleClicked(QModelIndex)), this, SLOT(slotDoubleClicked(QModelIndex)) );

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

    QVBoxLayout* layout = new QVBoxLayout(widget);
    layout->addItem( filterLayout );
    layout->addWidget( m_view );
    layout->addItem( hLayout );

    setCentralWidget( widget );
    setWindowTitle( i18n("Nepomuk Person Manager") );
    setWindowIcon( KIcon("nepomuk") );
}

QList<QUrl> MainWindow::selectedTags()
{
    QItemSelectionModel* selectedModel = m_view->selectionModel();
    if( !selectedModel->hasSelection() )
        return QList<QUrl>();

    QList<QUrl> list;
    int size = m_model->rowCount();
    for( int row=0; row<size; row++ ) {
        if( selectedModel->isRowSelected(row, QModelIndex()) ) {
            QModelIndex index = m_model->index(row);
            QString tagLabel = index.data().toString();
            Nepomuk::Resource res(tagLabel);
            list << res.resourceUri();
        }
    }

    return list;
}

void MainWindow::slotOnDelete()
{
    QList<QUrl> tags = selectedTags();
    if( tags.isEmpty() )
        return;

    // Show confirmation message
    int answer = KMessageBox::questionYesNo( this, i18n("Do you want to Delete these Tags?") );
    if( answer == KMessageBox::No )
        return;

    KJob* job = Nepomuk::removeResources( tags );
    connect( job, SIGNAL(finished(KJob*)), this, SLOT(slotOnDeleteJob(KJob*)) );
}

void MainWindow::slotOnDeleteJob(KJob* job)
{
    if( job->error() ) {
        kWarning() << job->errorString();
        KMessageBox::sorry(this, i18n("The tags could not be deleted"));
    }
    else {
        KMessageBox::messageBox( this, KMessageBox::Information, i18n("Tags sucessfully deleted") );
        //m_model->repopulate();
    }
}

void MainWindow::slotOnMerge()
{
    QList<QUrl> tags = selectedTags();
    if( tags.isEmpty() )
        return;

    // Show confirmation message
    int answer = KMessageBox::questionYesNo( this, i18n("Do you want to merge these tags?") );
    if( answer == KMessageBox::No )
        return;

    m_mergeTagsList = tags;
    QUrl first = m_mergeTagsList.first();
    m_mergeTagsList.pop_front();

    KJob *job = Nepomuk::mergeResources( first, m_mergeTagsList.first() );
    connect( job, SIGNAL(finished(KJob*)), this, SLOT(slotOnMergeJob(KJob*)) );
}

void MainWindow::slotOnMergeJob(KJob* job)
{
    if( job->error() ) {
        kWarning() << job->errorString();
        KMessageBox::sorry(this, i18n("The tags could not be merged"));
        return;
    }

    if( m_mergeTagsList.size() <= 1 ) {
        KMessageBox::messageBox( this, KMessageBox::Information, i18n("Tags sucessfully merged") );
 //       m_model->repopulate();
        return;
    }

    QUrl first = m_mergeTagsList.first();
    m_mergeTagsList.pop_front();

    KJob *kjob = Nepomuk::mergeResources( first, m_mergeTagsList.first() );
    connect( kjob, SIGNAL(finished(KJob*)), this, SLOT(slotOnMergeJob(KJob*)) );
}

void MainWindow::slotOnRename()
{
    QItemSelectionModel* selectedModel = m_view->selectionModel();
    if( !selectedModel->hasSelection() )
        return;

    QModelIndexList indexList = selectedModel->selectedRows();
    if( indexList.size() > 1 ) {
        KMessageBox::error( this, i18n("You can only rename one tag at a time") );
        return;
    }

    QModelIndex index = indexList.first();
    QString oldLabel = index.data().toString();

    KDialog *dialog = new KDialog( this );
    dialog->enableButtonApply( true );
    dialog->enableButtonCancel( true );
    QWidget* widget = new QWidget( dialog );
    QHBoxLayout* hLayout = new QHBoxLayout( widget );

    QLabel* label = new QLabel( i18n("New Label: "), widget );
    KLineEdit *lineEdit = new KLineEdit( oldLabel, widget );

    hLayout->addWidget( label );
    hLayout->addWidget( lineEdit );

    dialog->setMainWidget( widget );

    int returnCode = dialog->exec();
    if( returnCode == QDialog::Rejected )
        return;

    QString newLabel = lineEdit->text();
//    m_model->rename( index, newLabel );
}

void MainWindow::slotFilter(const QString& filter)
{
    m_filterModel->setFilterRegExp( filter );
}

void MainWindow::slotDoubleClicked(const QModelIndex& index)
{
    if( !index.isValid() )
        return;

}

void MainWindow::slotOnSelectionChange()
{
    int num = m_view->selectionModel()->selectedRows().size();
    m_renameMergeButton->disconnect();
    if( num <= 1 ) {
        m_renameMergeButton->setText(i18n("Rename Tag"));
        connect( m_renameMergeButton, SIGNAL(clicked(bool)), this, SLOT(slotOnRename()) );
    }
    else {
        m_renameMergeButton->setText(i18n("Merge Tags"));
        connect( m_renameMergeButton, SIGNAL(clicked(bool)), this, SLOT(slotOnMerge()) );
    }
}
