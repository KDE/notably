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

#include "mainwindow.h"

#include <KAboutData>
#include <KCmdLineArgs>
#include <KLocale>
#include <KComponentData>

#include <KUniqueApplication>
#include <KUrl>
#include <KIcon>

int main(int argc, char *argv[])
{
    KAboutData aboutData("notably", 0, ki18n("Notably"),
                         "0.3",
                         ki18n("A note taking application based on the Nepomuk framework"),
                         KAboutData::License_GPL_V2,
                         ki18n("(C) 2011, Vishesh Handa"));
    aboutData.addAuthor(ki18n("Vishesh Handa"), ki18n("Maintainer and developer"),
                        "handa.vish@gmail.com", "http://vhanda.in");

    KCmdLineArgs::init(argc, argv, &aboutData);
    KUniqueApplication app;
    app.setWindowIcon(KIcon("nepomuk"));

    //vHanda: Figure out how/why this is being deleted!
    MainWindow *mainWindow = new MainWindow();
    mainWindow->show();

    return app.exec();
}
