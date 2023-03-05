#include <string.h>

#include <bdb/db_cxx.h>
#include <mos/SequenceExample.h>

#define	SEQUENCE "my_sequence"

using std::cout;
using std::cerr;

SequenceExample::SequenceExample() = default;

void SequenceExample::run(bool removeExistingDatabase, const char* fileName)
{
    // Remove the previous database.
    if (removeExistingDatabase)
        (void)remove(fileName);

    // Create the database object.
    // There is no environment for this simple example.
    Db db(0, 0);

    db.set_error_stream(&cerr);
    db.set_errpfx("SequenceExample");
    db.open(NULL, fileName, NULL, DB_BTREE, DB_CREATE, 0664);

    // We put a try block around this section of code
    // to ensure that our database is properly closed
    // in the event of an error.
    //
    try {
        Dbt key((void*)SEQUENCE, (u_int32_t)strlen(SEQUENCE));
        DbSequence seq(&db, 0);
        seq.open(0, &key, DB_CREATE);

        for (int i = 0; i < 10; i++) {
            db_seq_t seqnum;
            seq.get(0, 1, &seqnum, 0);

            // We don't have a portable way to print 64-bit numbers.
            cout << "Got sequence number (" <<
                (int)(seqnum >> 32) << ", " << (unsigned)seqnum <<
                ")\n";
        }

        seq.close(0);
    }
    catch (DbException& dbe) {
        cerr << "SequenceExample: " << dbe.what() << "\n";
    }

    db.close(0);
}
