/*
 * This file is part of SemNotes Project.
 *
 * Copyright (C) 2009 - 2010 Laura Dragan <laura.dragan@deri.org>
 *                           Pierre Ludwick <pierre.ludwick@deri.org>
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Library General Public
 * License as published by the Free Software Foundation; either
 * version 2 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Library General Public License for more details.
 *
 * You should have received a copy of the GNU Library General Public License
 * along with this library; see the file COPYING.LIB.  If not, write to
 * the Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor,
 * Boston, MA 02110-1301, USA.
 */

#ifndef TAGEDITOR_H_
#define TAGEDITOR_H_

#include <nepomuk/tag.h>
#include <KLineEdit>

class QCompleter;
class QStringlist;

namespace SemNotes
{

class TagEditor : public KLineEdit
{
    Q_OBJECT

public:
    TagEditor(QWidget *parent = 0);
    TagEditor(QStringList options, QWidget *parent = 0);
    TagEditor(const QString &text, QStringList options, QWidget *parent = 0);
    ~TagEditor();
    virtual QSize sizeHint() const;
    void init(QStringList options);
    void hidePopup();

protected:
    void keyPressEvent(QKeyEvent *event);

private slots:
    void makeComplete(QString);
    void complete(QString completion);

private:
    QCompleter *m_completer;

};

}
#endif /*TAGEDITOR_H_*/
