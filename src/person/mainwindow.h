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


#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QtGui/QPushButton>
#include <QtGui/QWidget>
#include <QtGui/QMainWindow>
#include <QtGui/QListView>
#include <QtGui/QSortFilterProxyModel>

#include <Nepomuk/Query/Result>
#include <KLineEdit>
#include <KJob>

class PersonModel;

class MainWindow : public QMainWindow
{
    Q_OBJECT
public:
    explicit MainWindow(QWidget* parent = 0);

private slots:
    void slotOnDelete();
    void slotOnMerge();
    void slotOnRename();

    void slotOnDeleteJob(KJob* job);
    void slotOnMergeJob(KJob* job);

    void slotFilter(const QString& filter);
    void slotDoubleClicked(const QModelIndex& index);
    void slotOnSelectionChange();
private:
    QList<QUrl> selectedTags();

    PersonModel* m_model;
    QSortFilterProxyModel* m_filterModel;
    QListView *m_view;
    KLineEdit *m_filterBar;

    QPushButton* m_renameMergeButton;
    QList<QUrl> m_mergeTagsList;
};

#endif // MAINWINDOW_H
