#ifndef HINLIBSSYSTEM_H
#define HINLIBSSYSTEM_H

#include <vector>
#include <QString>
#include "catalogue.h"
#include "user.h"
#include "catalogueitemfactory.h"

struct BrowseItemInfo {
    int id;
    QString title;
    QString creator;
    QString format;
    QString statusText;
};

struct LoanInfo {
    int itemId;
    QString title;
    QString dueDateString;
    int daysRemaining;
};

struct HoldInfo {
    int itemId;
    QString title;
    int queuePosition;
};

// Aggregated account view returned to PatronWindow.
struct PatronAccountStatus {
    std::vector<LoanInfo> loans;
    std::vector<HoldInfo> holds;
};

struct OperationResult {
    bool success;
    QString message;
};

// Result of looking up a user by name at login.
struct FindUserResult {
    bool found;
    int userId;
    UserRole role;
};
struct PatronLoanInfo {
    int itemId;
    QString title;
    QString checkoutDate;
    QString dueDate;
};

class DbManager;

// Core application “system” class.
// Encapsulates domain objects (users, catalogue items) and all business rules.
// The GUI talks only to this class and never directly to the database.
class HinLIBSSystem
{
public:
    HinLIBSSystem();

    void loadDefaultData(); // 20 items + 7 users (in memory)
    FindUserResult findUserByName(const QString &name) const;
    explicit HinLIBSSystem(DbManager *dbManager);

    std::vector<BrowseItemInfo> browseCatalogue() const;
    PatronAccountStatus getPatronAccountStatus(int patronId) const;

    // Patron operations (borrow/return/holds), enforcing business rules.
    OperationResult borrowItem(int patronId, int itemId);   // max 3 loans, 14 days
    OperationResult returnItem(int patronId, int itemId);
    OperationResult placeHold(int patronId, int itemId);    // FIFO holds
    OperationResult cancelHold(int patronId, int itemId);

    // Librarian operations for managing the catalogue.
    OperationResult addCatalogueItem(const QString &title,
                                     const QString &creator,
                                     int year,
                                     const QString &isbn,
                                     const QString &typeStr,
                                     const QString &dewey,
                                     const QString &issueNumber,
                                     const QDate &pubDate,
                                     const QString &genre,
                                     const QString &rating); //add item to catalogue
    OperationResult removeCatalogueItem(int itemId); //this is to remove item from catalogue
    OperationResult findPatronByName(const QString &name, int &patronIdOut);
    std::vector<PatronLoanInfo> getPatronLoans(int patronId) const;
    OperationResult returnItemForPatron(int patronId, int itemId);


    Catalogue &catalogue() { return m_catalogue; }
    const Catalogue &catalogue() const { return m_catalogue; }

    // Load all data from SQLite into the in-memory model.
    void loadFromDatabase();

private:
    DbManager *m_dbManager; // Not owned; managed by main().
    Catalogue m_catalogue; // In-memory catalogue of items.
    std::vector<User> m_users;
    DefaultCatalogueItemFactory m_factory; // Creates concrete CatalogueItem objects.

    int m_nextItemId; // Next id to assign to new catalogue items.
    int m_nextUserId; // Next id to assign to new users

    const User *findUserById(int id) const;
    User *findUserById(int id);

    int findHoldPosition(const CatalogueItem *item, int patronId) const;
};

#endif // HINLIBSSYSTEM_H
