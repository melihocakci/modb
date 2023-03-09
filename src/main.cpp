/*-
 * Copyright (c) 1997, 2020 Oracle and/or its affiliates.  All rights reserved.
 *
 * See the file EXAMPLES-LICENSE for license information.
 *
 * $Id$
 */

#include <sys/types.h>

#include <iostream>
#include <errno.h>
#include <stdlib.h>

#ifdef _WIN32
extern "C" {
    extern int getopt(int, char* const*, const char*);
    extern int optind;
}
#else
#include <unistd.h>
#endif

#include <berkeleydb/db_cxx.h>
#include <mos/SequenceExample.h>
#include <spatialindex/SpatialIndex.h>

#define	DATABASE "sequence.db"

using std::cout;
using std::cerr;

int
usage()
{
    (void)fprintf(stderr, "usage: SequenceExample [-r] [database]\n");
    return (EXIT_FAILURE);
}

int
main(int argc, char* argv[])
{
    Tools::PropertySet* ps;

    int ch, rflag;
    const char* database;

    rflag = 0;
    while ((ch = getopt(argc, argv, "r")) != EOF)
        switch (ch) {
        case 'r':
            rflag = 1;
            break;
        case '?':
        default:
            return (usage());
        }
    argc -= optind;
    argv += optind;

    /* Accept optional database name. */
    database = *argv == NULL ? DATABASE : argv[0];

    // Use a try block just to report any errors.
    // An alternate approach to using exceptions is to
    // use error models (see DbEnv::set_error_model()) so
    // that error codes are returned for all Berkeley DB methods.
    //
    try {
        SequenceExample app;
        app.run((bool)(rflag == 1 ? true : false), database);
        return (EXIT_SUCCESS);
    }
    catch (DbException& dbe) {
        cerr << "SequenceExample: " << dbe.what() << "\n";
        return (EXIT_FAILURE);
    }
}