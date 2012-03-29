/*
   This file is part of the Nepomuk Notes project.
   Copyright (C) 2011  Vishesh Handa <handa.vish@gmail.com>

   This program is free software; you can redistribute it and/or
   modify it under the terms of the GNU General Public License as
   published by the Free Software Foundation; either version 2 of
   the License or (at your option) version 3 or any later version
   accepted by the membership of KDE e.V. (or its successor approved
   by the membership of KDE e.V.), which shall act as a proxy
   defined in Section 14 of version 3 of the license.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/


#ifndef NOTEEDIT_H
#define NOTEEDIT_H

#include <KTextEdit>
#include <QtGui/QCompleter>
#include <QtCore/QSet>
#include <Nepomuk/Resource>

#include <nepomuk/annotation.h>

class PersonCompleter;
class NoteDocument;
class TextAnnotation;
class TextAnnotationGroup;

class NoteEdit : public KTextEdit
{
    Q_OBJECT
public:
    explicit NoteEdit(QWidget* parent = 0);
    virtual ~NoteEdit();

    void reset();

    void setResource( const Nepomuk::Resource &note );
    Nepomuk::Resource resource() const;
signals:
    void annotationGroupSelected(TextAnnotationGroup *tag);

public slots:
    /**
     * Saves the note.
     * By saving the note, you update the nao:lastModified of the note. Do not save if you haven't
     * made any modifications, as that will unnecessarily update the modification time.
     */
    void save();

    void acceptAnnotation(TextAnnotation* ta);
    void rejectAnnotationGroup(TextAnnotationGroup *tag);

protected:
    virtual void keyPressEvent(QKeyEvent* event);
    virtual void mousePressEvent(QMouseEvent* e);

private slots:
    void insertCompletion(const QString& string);

    void slotNewAnnotation(Nepomuk::Annotation* annotation);
    void slotAnnotationsFinished();

private:
    QString wordUnderCursor() const;
    void insertAnnotation(TextAnnotation* annotation);
    void insertGroupAnnotation(TextAnnotationGroup* tag);

    Nepomuk::Resource m_noteResource;
    PersonCompleter* m_completer;

    NoteDocument* m_document;

    QMultiHash<int, TextAnnotation*> m_annotations;
};

#endif // NOTEEDIT_H
