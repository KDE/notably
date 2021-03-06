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


#ifndef TAGEDITOR_H
#define TAGEDITOR_H

#include <QtCore/QList>
#include <QtGui/QCompleter>

#include <KTextEdit>

#include <Nepomuk2/Tag>

class TagEditor : public KTextEdit
{
    Q_OBJECT
public:
    explicit TagEditor(QWidget* parent = 0);
    virtual ~TagEditor();

    void addTag( const Nepomuk2::Tag &tag );
    void setTags( const QList<Nepomuk2::Tag> &tags );
    QList<Nepomuk2::Tag> tags() const;

    virtual QSize sizeHint() const;

    void setPlaceholderText(const QString &text);
    QString placeholderText() const;

    virtual void reset();
signals:
    void tagsChanged( const QList<Nepomuk2::Tag> &tagList );

protected:
//    virtual void paintEvent(QPaintEvent* event);
    virtual void keyPressEvent(QKeyEvent* event);

private slots:
    void slotDocumentSizeChanged();
    void insertCompletion(const QString& completion);

private:
    QString tagUnderCursor() const;

    QList<Nepomuk2::Tag> m_tagList;
    QCompleter *m_completer;

    QString m_placeholderText;
};

#endif // TAGEDITOR_H
