#include <QApplication>
#include "hinlibssystem.h"
#include "loginwindow.h"
#include "dbmanager.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    // Open the SQLite database file in the project folder.

    DbManager db("/home/student/Team_50_D2/Team_50_D2/hinlibs.sqlite3"); //path to the SQL lite database.
    if (!db.isOpen()) {
            // Optional: show an error and exit.
        }
    HinLIBSSystem system(&db);
    system.loadFromDatabase(); // implement this to read tables

    LoginWindow w(&system);
    w.show();

    return a.exec();
}
