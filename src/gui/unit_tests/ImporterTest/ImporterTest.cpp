/*

                          Firewall Builder

                 Copyright (C) 2010 NetCitadel, LLC

  Author:  Roman Bovsunivskiy     a2k0001@gmail.com

  $Id$

  This program is free software which we release under the GNU General Public
  License. You may redistribute and/or modify this program under the terms
  of that license as published by the Free Software Foundation; either
  version 2 of the License, or (at your option) any later version.

  This program is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
  GNU General Public License for more details.

  To get a copy of the GNU General Public License, write to the Free Software
  Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA

*/

#include "ImporterTest.h"

#include "../../config.h"
#include "global.h"

#ifdef HAVE_LOCALE_H
#include <locale.h>
#endif

#include <fstream>
#include <iostream>
#include <algorithm>
#include <functional>
#include <stdexcept>

#include <qstring.h>

#ifndef _WIN32
#  include <unistd.h>
#  include <pwd.h>
#else
#  include <direct.h>
#  include <stdlib.h>
#  include <io.h>
#endif

#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <ctype.h>
#include <assert.h>

#include "../../Importer.h"
#include "../../IOSImporter.h"
#include "../../IPTImporter.h"

#include "../../FWBTree.h"


#ifdef HAVE_GETOPT_H
  #include <getopt.h>
#else
  #ifdef _WIN32
    #include <getopt.h>
  #else
    #include <stdlib.h>
  #endif
#endif

#include <QDebug>
#include <QFile>
#include <QStringList>



using namespace std;
using namespace libfwbuilder;

extern string platform;

class UpgradePredicate: public XMLTools::UpgradePredicate
{
    public:
    virtual bool operator()(const string &) const
    {
        return false;
    }
};

void ImporterTest::setUp()
{
    //init();

    string full_res_path = respath + FS_SEPARATOR + "resources.xml";
    new Resources(full_res_path);

    new FWBTree();

    /* create database */
    db = new FWObjectDatabase();

    /* load the data file */
    UpgradePredicate upgrade_predicate;

    db->setReadOnly( false );

    //path.pop_back();
    //path << "objects_init.xml";
    //sysfname = path.join(FS_SEPARATOR).toStdString();
    //librespath = string(PREFIX) + "/share/libfwbuilder-" + VERSION;

    qDebug() << sysfname.c_str();
    qDebug() << librespath.c_str();

    db->load( sysfname, &upgrade_predicate, librespath);
    qDebug() << "st";

    db->setFileName("");
    lib = Library::cast(db->create(Library::TYPENAME));
    lib->setName("User");
    db->add(lib);

    logger = new QueueLogger();
}

void ImporterTest::compareResults(QueueLogger* logger,
                                  QString expected_result_file_name,
                                  QString obtained_result_file_name)
{
    QString result;
    while (logger->ready())
        result.append(logger->getLine().c_str());
    QStringList obtained_result = result.split("\n");

    QFile rr(expected_result_file_name);
    rr.open(QFile::ReadOnly);
    QString result_file = rr.readAll();
    QStringList expected_result = result_file.split("\n");

    QFile rw(obtained_result_file_name);
    rw.open(QFile::WriteOnly);
    foreach (QString line, obtained_result)
        rw.write(line.toAscii());
    rw.close();

    int max_idx = max(expected_result.size(), obtained_result.size());
    for (int i=0; i < max_idx; ++i)
    {
        QString err = QString("Line %1:\nExpected: '%2'\nResult: '%3'\n")
            .arg(i).arg(expected_result[i]).arg(obtained_result[i]);
        CPPUNIT_ASSERT_MESSAGE(err.toStdString(), obtained_result[i] == expected_result[i]);
    }
}

void ImporterTest::IOSImporterTest()
{
    platform = "iosacl";

    QFile f("test_data/ios.test");
    f.open(QFile::ReadOnly);

    string buffer = QString(f.readAll()).toStdString();
    f.close();

    std::istringstream instream(buffer);

    Importer* imp = new IOSImporter(lib, instream, logger);

    CPPUNIT_ASSERT_NO_THROW( imp->run() );
    //imp->run();

    imp->finalize();
    compareResults(logger, "test_data/ios.result", "ios.output");
}

void ImporterTest::IPTImporterTest()
{
    platform = "iptables";

    QFile f("test_data/ipt.test");
    f.open(QFile::ReadOnly);

    string buffer = QString(f.readAll()).toStdString();
    f.close();

    std::istringstream instream(buffer);

    Importer* imp = new IPTImporter(lib, instream, logger);

    CPPUNIT_ASSERT_NO_THROW( imp->run() );
    //imp->run();

    imp->finalize();
    compareResults(logger, "test_data/ipt.result", "ipt.output");
}
