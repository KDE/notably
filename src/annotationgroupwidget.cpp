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


#include "annotationgroupwidget.h"
#include "person/person.h"
#include <KPushButton>
#include <KDebug>

#include <QtCore/QAbstractListModel>
#include <QtGui/QListView>
#include <QtGui/QBoxLayout>
#include <QtGui/QRadioButton>

class AnnotationGroupModel: public QAbstractListModel {
public:
    explicit AnnotationGroupModel(TextAnnotationGroup* tag, QObject* parent = 0);

    QVariant data(const QModelIndex& index, int role = Qt::DisplayRole) const;
    int rowCount(const QModelIndex& parent = QModelIndex()) const;

private:
    TextAnnotationGroup* m_group;
};


QVariant AnnotationGroupModel::data(const QModelIndex& index, int role) const
{
    if( !index.isValid() || index.row() >= m_group->annotations().length() )
        return QVariant();

    TextAnnotation* ann = m_group->annotations().at( index.row() );
    switch( role ) {
        case Qt::DisplayRole:
            return Person(ann->object().resourceUri()).nickName();
    }

    return QVariant();
}

int AnnotationGroupModel::rowCount(const QModelIndex& parent) const
{
    return m_group->annotations().size();
}

AnnotationGroupModel::AnnotationGroupModel(TextAnnotationGroup* tag, QObject* parent)
    : QAbstractListModel(parent)
    , m_group( tag )
{
}

//
//
//

AnnotationGroupWidget::AnnotationGroupWidget(TextAnnotationGroup* tag, QWidget* parent)
    : QWidget(parent)
    ,  m_group( tag )
{
    kDebug() << tag->annotations().size();
    QVBoxLayout* vLayout = new QVBoxLayout( this );

    QList<TextAnnotation*> tal = tag->annotations();
    foreach( TextAnnotation* ta, tal ) {
        QString name = Person(ta->object().resourceUri()).nickName();
        QRadioButton* rb = new QRadioButton( name );

        vLayout->addWidget( rb );
        m_buttons << rb;
    }

    KPushButton* acceptButton = new KPushButton( i18n("Accept"), this );
    KPushButton* rejectButton = new KPushButton( i18n("Reject"), this );

    vLayout->addWidget( acceptButton );
    vLayout->addWidget( rejectButton );

    connect( acceptButton, SIGNAL(clicked(bool)), this, SLOT(slotOnAccept()) );
    connect( rejectButton, SIGNAL(clicked(bool)), this, SLOT(slotOnReject()) );
}

AnnotationGroupWidget::~AnnotationGroupWidget()
{

}

void AnnotationGroupWidget::slotOnAccept()
{
    TextAnnotation* ta = 0;
    for( int i=0; i<m_buttons.size(); i++ ) {
        QRadioButton* rb = m_buttons[i];

        if( rb->isChecked() ) {
            ta = m_group->annotations()[i];
            break;
        }
    }

    emit acceptAnnotation( ta );
}

void AnnotationGroupWidget::slotOnReject()
{
    emit rejectAnnotations( m_group );
}

