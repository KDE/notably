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

#include <QtCore/QSet>

#include <KTextEdit>

#include <Nepomuk/Tag>

class TagEditor : public KTextEdit
{
    Q_OBJECT
public:
    explicit TagEditor(QWidget* parent = 0);
    virtual ~TagEditor();

    void addTag( const Nepomuk::Tag &tag );
    void setTags( const QSet<Nepomuk::Tag> &tags );
    QSet<Nepomuk::Tag> tags() const;

    virtual QSize sizeHint() const;

signals:
    void tagsChanged( const QSet<Nepomuk::Tag> &tagList );

protected:
    virtual void paintEvent(QPaintEvent* event);
    virtual void keyPressEvent(QKeyEvent* event);

private slots:
    void slotDocumentSizeChanged();

private:
    QSet<Nepomuk::Tag> m_tagList;
};

#endif // TAGEDITOR_H
