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


#ifndef TAGCLOUD_H
#define TAGCLOUD_H

#include <QtGui/QTextBrowser>
#include <QtGui/QPaintEvent>
#include <QtCore/QMap>

#include <Nepomuk2/Tag>


class TagCloud : public QTextBrowser
{
    Q_OBJECT
public:
    explicit TagCloud(QWidget* parent = 0);

signals:
    void tagSelected(const Nepomuk2::Tag& tag);

private slots:
    void slotAnchorClicked(const QUrl& url);

private:
    void createBrowser();

    QMap<QString, float> m_hashMap;
    int m_maxFontSize;
    int m_minFontSize;
    int m_spacing;
};

#endif // TAGCLOUD_H
