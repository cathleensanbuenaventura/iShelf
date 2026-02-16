#include "dbmanager.h"
#include <QSqlError>
#include <QDebug>

// DbManager: wrapper around QSqlDatabase that owns the SQLite connection
// it is used by the rest of the application. It opens the database in the constructor
// and closes it in the destructor.
DbManager::DbManager(const QString &dbPath)
{
    // Use the default connection name.
    m_db = QSqlDatabase::addDatabase("QSQLITE");
    m_db.setDatabaseName(dbPath);

    if (!m_db.open()) {
        qWarning() << "Failed to open database" << dbPath
                   << ":" << m_db.lastError().text();
    }
}

DbManager::~DbManager()
{
    if (m_db.isOpen()) {
        m_db.close();
    }
}

bool DbManager::isOpen() const
{
    return m_db.isOpen();
}

// Return the underlying QSqlDatabase object so other classes (e.g. HinLIBSSystem)
// can create QSqlQuery objects and execute SQL statements.
QSqlDatabase DbManager::database() const
{
    return m_db;
}
