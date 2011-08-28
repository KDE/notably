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

#include "tageditor.h"

#include <KDebug>

#include <QtGui/QAbstractItemView>
#include <QtGui/QCompleter>
#include <QtGui/QKeyEvent>
#include <QtGui/QTextCursor>

#include <Nepomuk/Tag>

namespace SemNotes
{

/**
 * \brief Widget that allows the user to input comma separated tags, with text completion based on
 * existing tags.
 *
 * \details The widget is an extension of QLineEdit, with an extra completer, to give suggestions
 * for each comma separated word(s) based on the list given as parameter at initialization. The words input
 * by the user are parsed when the widget loses focus, and are set as tags to the current note. If the tag
 * instance does not exist, it is created.
 *
 */

TagEditor::TagEditor(QWidget *parent)
        : KLineEdit(parent)
{
}

TagEditor::TagEditor(QStringList options, QWidget *parent)
        : KLineEdit(parent)
{
    init(options);
}

TagEditor::TagEditor(const QString &text, QStringList options, QWidget *parent)
        : KLineEdit(text, parent)
{
    init(options);
}

TagEditor::~TagEditor()
{
}

/**
 * Sets the wrap mode to the widget width, sets the size policy - expanding horizontaly, fixed verticaly.
 * Creates the completer based on the list provided in the constructor, and connects it to the widget.
 */
void TagEditor::init(QStringList options)
{
    // create the completer
    m_completer = new QCompleter(options, this);

    // connect the completer to the textedit
    m_completer->setWidget(this);
    connect(m_completer, SIGNAL(activated(QString)), this, SLOT(complete(QString)));
    connect(this, SIGNAL(textChanged(QString)), this, SLOT(makeComplete(QString)));

    setClickMessage("Tags (comma separated)");
}

/**
 * Makes the completer pop-up the posible completions, setting the prefix to the current tag that's edited
 * (get this tag by parsing the text of the editor and extracting the word(s) where the cursor is at, limited
 * by text margins or commas) - all the rest of the text remains unchanged
 */
void TagEditor::makeComplete(QString text)
{
    int c = this->cursorPosition();
    int prev = text.lastIndexOf(",", c - 1);
    int next = text.indexOf(",", c);

    if (next == -1)
        next = text.length();
    QString prefix = text.left(next).remove(0, prev + 1).trimmed();

    if (prefix.length() > 0) {
        m_completer->setCompletionPrefix(prefix);
        m_completer->complete();
    }
}

/**
 * Adds the chosen completion to the text in the box and hides the pop-up
 */
void TagEditor::complete(QString completion)
{
    QString prefix = m_completer->completionPrefix();
    this->setText(this->text().replace((this->cursorPosition() - prefix.length()), prefix.length(), completion));
    this->setModified(true);
    if (m_completer && m_completer->popup() && m_completer->popup()->isVisible()) {
        hidePopup();
    }
}

/**
 * Forwards the relevant key events to the completer, if a pop-up is shown,
 * otherwise lets the textedit process them.
 */
void TagEditor::keyPressEvent(QKeyEvent *event)
{
    if (m_completer && m_completer->popup() && m_completer->popup()->isVisible()) {
        switch (event->key()) {
        case Qt::Key_Escape:
            event->ignore();
            return;
        case Qt::Key_Enter:
            event->ignore();
            return;
        case Qt::Key_Return:
            event->ignore();
            return;
        case Qt::Key_F4:
            event->ignore();
        case Qt::Key_Select:
            hidePopup();
        default:
            QLineEdit::keyPressEvent(event);
        }
    } else {
        QLineEdit::keyPressEvent(event);
    }
}

QSize TagEditor::sizeHint() const
{
    //int height = QFontMetrics(document()->defaultFont()).lineSpacing();
    return QSize(50, 1 * this->size().height());
}

void TagEditor::hidePopup()
{
    m_completer->popup()->hide();
}

}
#include "tageditor.moc"
