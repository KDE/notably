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


#ifndef NOTEWIDGET_H
#define NOTEWIDGET_H

#include <QtGui/QWidget>

#include <Nepomuk/Resource>

class NoteEdit;
class TagEditor;

class NoteWidget : public QWidget
{
    Q_OBJECT
public:
    explicit NoteWidget(QWidget* parent = 0, Qt::WindowFlags f = 0);
    virtual ~NoteWidget();

    /**
     * The existing note is saved, but creating a new note.
     */
    void newNote();

    /**
     * Save the note along with the associated tags and resources.
     * The note is only saved if some modifications have been made.
     */
    void saveNote();

    void reset();

public slots:
    void setNote(const Nepomuk::Resource& res);

private:
    Nepomuk::Resource lastUsedNote() const;

    NoteEdit *m_noteEditor;
    TagEditor *m_tagEditor;
};

#endif // NOTEWIDGET_H
