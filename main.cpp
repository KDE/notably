/*
   This file is part of the Nepomuk KDE project.
   Copyright (C) 2011 Vishesh Handa <handa.vish@gmail.com>

   This library is free software; you can redistribute it and/or
   modify it under the terms of the GNU Lesser General Public
   License as published by the Free Software Foundation; either
   version 2.1 of the License, or (at your option) version 3, or any
   later version accepted by the membership of KDE e.V. (or its
   successor approved by the membership of KDE e.V.), which shall
   act as a proxy defined in Section 6 of version 3 of the license.

   This library is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
   Lesser General Public License for more details.

   You should have received a copy of the GNU Lesser General Public
   License along with this library.  If not, see <http://www.gnu.org/licenses/>.
*/

#include "mainwindow.h"

#include <KAboutData>
#include <KCmdLineArgs>
#include <KLocale>
#include <KComponentData>

#include <KUniqueApplication>
#include <KUrl>

int main(int argc, char *argv[])
{
    KAboutData aboutData("nnotes", 0, ki18n("NepomukNotes"),
                         "0.1",
                         ki18n("A simple note taking application using Nepomuk"),
                         KAboutData::License_LGPL_V2,
                         ki18n("(C) 2011, Vishesh Handa"));
    aboutData.addAuthor(ki18n("Vishesh Handa"), ki18n("Maintainer and developer"), "handa.vish@gmail.com");

    KCmdLineArgs::init(argc, argv, &aboutData);
    KUniqueApplication app;

    //vHanda: Figure out how/why this is being deleted!
    MainWindow *mainWindow = new MainWindow();
    mainWindow->show();

    return app.exec();
}
