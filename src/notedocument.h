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


#ifndef NOTEDOCUMENT_H
#define NOTEDOCUMENT_H

#include <QtGui/QTextDocument>
#include <QtCore/QSet>
#include <QtCore/QUrl>

class NoteDocument : public QTextDocument
{
    Q_OBJECT
public:
    explicit NoteDocument(QObject* parent = 0);
    virtual ~NoteDocument();

    /**
     * Converts the plain text to very simple html and embed RDFa information
     * to the annotated text
     */
    QString toRDFaHtml() const;

    void setRDFaHtml(const QString& html);

    QSet<QUrl> resources(const QUrl& property);
    QUrl resourceAtCursor(const QTextCursor& tc);

private slots:
    void slotContentsChanged();

private:
    QString m_htmlContent;
};

#endif // NOTEDOCUMENT_H
