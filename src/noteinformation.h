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


#ifndef NOTEINFORMATION_H
#define NOTEINFORMATION_H

#include <QtGui/QWidget>
#include <QtGui/QLabel>

#include <Nepomuk/Resource>
#include <Nepomuk/Tag>

#include <KLineEdit>

class TagWidget;
class PersonGrid;

namespace Nepomuk {
    class Annotation;
}

class NoteInformation : public QWidget
{
    Q_OBJECT
public:
    explicit NoteInformation(QWidget* parent = 0, Qt::WindowFlags f = 0);


public slots:
    void setNote( const Nepomuk::Resource& note );
    bool saveNote( const Nepomuk::Resource& note );
    void newNote();

signals:
    void tagSelected(const Nepomuk::Tag& tag);
    void personSelected(const Nepomuk::Resource& person);

private slots:
    void updateView();
    void slotAnnotateClicked();
    void slotNewAnnotation(Nepomuk::Annotation* annotation);

private:
    Nepomuk::Resource m_note;

    KLineEdit* m_titleEdit;
    QLabel* m_modifiedLabel;
    QLabel* m_createdLabel;

    TagWidget* m_tagWidget;
    PersonGrid* m_personGrid;
};

#endif // NOTEINFORMATION_H
