/*************************************************************************
* Copyright © 2011 Vincent Prat & Simon Nicolas
*
* This program is free software; you can redistribute it and/or modify
* it under the terms of the GNU General Public License as published by
* the Free Software Foundation; either version 3 of the License, or
* (at your option) any later version.
*
* This program is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
* GNU General Public License for more details.
*
* You should have received a copy of the GNU General Public License along
* with this program; if not, write to the Free Software Foundation, Inc.,
* 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.
*************************************************************************/

#include <QApplication>
#include <QTranslator>
#include <QLocale>
#include <QTextCodec>
#include <QLibraryInfo>

#ifdef __linux__
    #include <fstream>
    #include <iostream>
#endif

#include "MainWindow.h"

#define MAX_LENGTH  256

#undef main

using namespace std;

int main(int argc, char* argv[])
{
    QApplication app(argc, argv);

    // encoding
    QTextCodec::setCodecForLocale(QTextCodec::codecForName("UTF-8"));
    QTextCodec::setCodecForCStrings(QTextCodec::codecForName("UTF-8"));
    QTextCodec::setCodecForTr(QTextCodec::codecForName("UTF-8"));

    // getting install directory (on Linux only)
    QString install_dir;
#ifdef __linux__
    char buf[MAX_LENGTH];
    string home_dir = getenv("HOME");
    try
    {
        ifstream file((home_dir+"/.GM-Assistant").c_str()); 
        if (!file)
        {
            throw 0;
        }
        file.getline(buf,MAX_LENGTH);
        file.close();
        install_dir = buf;
    }
    catch (...)
    {
        cerr << "Unable to read the configuration file" << endl;
        install_dir = "";
    }
#endif

    QString locale = QLocale::system().name().section('_',0,0);
    QTranslator translator,translatorSys;
    // Translation of the software
    translator.load(install_dir + "/translations/gmassistant_" + locale);
    app.installTranslator(&translator);
    // Translation of predefined Qt strings
    translatorSys.load("qt_" + locale, QLibraryInfo::location(QLibraryInfo::TranslationsPath));
    app.installTranslator(&translatorSys);

    // Display of the main window
    MainWindow main(install_dir);
    main.show();

    return app.exec();
}
