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

#ifndef PORTERSTEMMER_H
#define PORTERSTEMMER_H

#include <QtCore/QString>
#include <KDebug>

namespace Porter {
    struct stemmer;

    extern struct stemmer * create_stemmer(void);
    extern void free_stemmer(struct stemmer * z);

    extern int stem(struct stemmer * z, char * b, int k);

    struct stemmer {
        char * b;       /* buffer for word to be stemmed */
        int k;          /* offset to the end of the string */
        int j;          /* a general offset into the string */
    };
}

inline QString stem( const QString& string ) {
    Porter::stemmer *z = Porter::create_stemmer();

    QByteArray bytes = string.trimmed().toUtf8();

    int len = bytes.size();
    char * newString = new char[ len+1 ];
    for( int i=0; i<len; i++ )
        newString[i] = bytes[i];
    newString[ len ] = '\0';

    int newLen = Porter::stem( z, newString+1, len-1 );
    newString[ newLen ] = '\0';

    QString stemmedString = QString::fromUtf8( newString, newLen );

    delete [] newString;
    Porter::free_stemmer( z );

    return stemmedString;
}

#endif