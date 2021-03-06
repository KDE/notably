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


#ifndef SIDEBAR_H
#define SIDEBAR_H

#include <QtGui/QWidget>
#include <QtGui/QAbstractItemView>
#include <QtGui/QSortFilterProxyModel>
#include <QtGui/QStackedLayout>
#include <QtGui/QLabel>
#include <QtGui/QPushButton>

#include <Nepomuk2/Resource>
#include <Nepomuk2/Tag>

class TextAnnotation;
class MainMenu;
class NoteInformation;
class TextAnnotationGroup;

class Sidebar : public QWidget
{
    Q_OBJECT
public:
    explicit Sidebar(QWidget* parent = 0, Qt::WindowFlags f = 0);
    virtual ~Sidebar();

public slots:
    void setTitle(const QString& title);
    void push(const QString& title, QWidget* widget);
    void showInfo(const Nepomuk2::Resource &note);
    void showAnnotationGroup(TextAnnotationGroup* tag);

    bool saveNote(const Nepomuk2::Resource& note);
    void newNote();

signals:
    void noteSelected(const Nepomuk2::Resource &note);
    void newNoteSelected();

    void annotationAccepted(TextAnnotation* annotation);
    void annotationRejected(TextAnnotationGroup* group);

private slots:
    void slotBrowseNotes();
    void slotBrowseByDate();
    void slotBrowseByTags();

    void slotMoveForward();
    void slotMoveBackward();

    void showTagInBrowser(const Nepomuk2::Tag& tag);
    void showPersonInBrowser(const Nepomuk2::Resource& resource);
private:
    void updateButtons();

    MainMenu *m_mainMenu;
    NoteInformation *m_noteInfo;

    QStackedLayout *m_stackedLayout;
    QStringList m_titleList;

    QLabel *m_title;
    QPushButton *m_backButton;
    QPushButton *m_forwardButton;
};

#endif // SIDEBAR_H
