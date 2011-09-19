/*
    <one line to give the library's name and an idea of what it does.>
    Copyright (C) 2011  Vishesh Handa <handa.vish@gmail.com>
    Copyright (C) 2009  Peter Penz <peter.penz@gmx.at>

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


#include "notesview.h"
#include "noteitemdelegate.h"

#include <QtCore/QEvent>

#include <KLocale>
#include <KMessageBox>

#include <Nepomuk/Resource>
#include <Nepomuk/Variant> // For Q_DECLARE_METATYPE<Nepomuk::Resource>
#include <Nepomuk/Utils/SimpleResourceModel>
#include <KDebug>

NotesView::NotesView(QWidget* parent)
    : QListView(parent)
{
    setItemDelegate( new NoteItemDelegate(this) );

    setMouseTracking( true );
    connect( this, SIGNAL(entered(QModelIndex)), this, SLOT(slotItemEntered(QModelIndex)) );

    m_deleteButton = new KPushButton(viewport());
    m_deleteButton->setIcon(KIcon("edit-delete"));
    m_deleteButton->setToolTip(i18nc("@info", "Delete tag"));
    m_deleteButton->hide();
    connect(m_deleteButton, SIGNAL(clicked()), this, SLOT(deleteNote()));

    m_deleteButtonTimer = new QTimer(this);
    m_deleteButtonTimer->setSingleShot(true);
    m_deleteButtonTimer->setInterval(500);
    connect(m_deleteButtonTimer, SIGNAL(timeout()), this, SLOT(showDeleteButton()));

    // Misc
    setUniformItemSizes( true );
    setAlternatingRowColors( true );
}

NotesView::~NotesView()
{
}

void NotesView::showDeleteButton()
{
    m_deleteButton->show();
}

void NotesView::slotItemEntered(const QModelIndex& index)
{
    // align the delete-button to stay on the right border
    // of the item
    const QRect rect = visualRect(index);
    const int size = m_deleteButton->size().height();
    const int x = rect.right() - size;
    const int y = rect.top();
    m_deleteButton->setGeometry(x, y, size, size);

    m_deleteCandidate = index;
    m_deleteButtonTimer->start();
}

void NotesView::deleteNote()
{
    if( !m_deleteCandidate.isValid() )
        return;

    const QString text = i18nc("@info", "Do you want to permanently delete this note?");
    const QString caption = i18nc("@title", "Delete Note");
    const KGuiItem deleteItem(i18nc("@action:button", "Delete"), KIcon("edit-delete"));
    const KGuiItem cancelItem(i18nc("@action:button", "Cancel"), KIcon("dialog-cancel"));

    if (KMessageBox::warningYesNo(this, text, caption, deleteItem, cancelItem) == KMessageBox::Yes) {

        qRegisterMetaType<Nepomuk::Resource>();
        Nepomuk::Resource res = m_deleteCandidate.data( Nepomuk::Utils::SimpleResourceModel::ResourceRole ).value<Nepomuk::Resource>();

        kDebug() << "Deleting : " << res.resourceUri();

        // FIXME: The Nepomuk ResourceModel doesn't support removing of rows! Use some other model
        //        => The view isn't updated corrected
        kDebug() << model()->removeRow( m_deleteCandidate.row() );
        res.remove();

        // Reset the candidate
        m_deleteCandidate = QModelIndex();
    }
}

void NotesView::leaveEvent(QEvent* event)
{
    m_deleteButtonTimer->stop();
    m_deleteButton->hide();

    QWidget::leaveEvent(event);
}
