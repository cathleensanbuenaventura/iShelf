#ifndef DBMANAGER_H
#define DBMANAGER_H

#include <QSqlDatabase>
#include <QString>

// helper that owns the QSqlDatabase connection to hinlibs.sqlite3.
class DbManager
{
public:
    // dbPath is usually "hinlibs.sqlite3" in the project folder.
    explicit DbManager(const QString &dbPath);

    ~DbManager();

    // True if the database opened successfully.
    bool isOpen() const;

    // Gives access to the underlying QSqlDatabase so other classes can use it.
    QSqlDatabase database() const;

private:
    QSqlDatabase m_db;
};

#endif // DBMANAGER_H
