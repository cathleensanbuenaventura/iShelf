#include "hinlibssystem.h"
#include "dbmanager.h"
#include <QDate>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlError>
#include <QVariant>
#include <QDebug>

HinLIBSSystem::HinLIBSSystem()
    : m_nextItemId(1),
      m_nextUserId(1)
{
}

HinLIBSSystem::HinLIBSSystem(DbManager *dbManager)
    : m_dbManager(dbManager),
      m_nextItemId(1),
      m_nextUserId(1)
{
}

//this was D1 catalogue and user data saved in memory.. D2 is using the database
void HinLIBSSystem::loadDefaultData()
{
    // 5 fiction books
    m_catalogue.addItem(m_factory.createFictionBook(
        m_nextItemId++, "The Great Adventure", "A. Writer", 2010, "1111111111"));
    m_catalogue.addItem(m_factory.createFictionBook(
        m_nextItemId++, "Mystery of Hintonville", "B. Author", 2015, "2222222222"));
    m_catalogue.addItem(m_factory.createFictionBook(
        m_nextItemId++, "Lost in the Stacks", "C. Novelist", 2018, "3333333333"));
    m_catalogue.addItem(m_factory.createFictionBook(
        m_nextItemId++, "The Last Checkout", "D. Storyteller", 2020, "4444444444"));
    m_catalogue.addItem(m_factory.createFictionBook(
        m_nextItemId++, "Return of the Books", "E. Writer", 2022, "5555555555"));

    // 5 non-fiction with Dewey XXX.XX
    m_catalogue.addItem(m_factory.createNonFictionBook(
        m_nextItemId++, "Intro to Databases", "F. Scholar", 2012, "6666666666", "005.74"));
    m_catalogue.addItem(m_factory.createNonFictionBook(
        m_nextItemId++, "Library Management", "G. Expert", 2014, "7777777777", "025.10"));
    m_catalogue.addItem(m_factory.createNonFictionBook(
        m_nextItemId++, "AI Safety", "H. Researcher", 2021, "8888888888", "006.30"));
    m_catalogue.addItem(m_factory.createNonFictionBook(
        m_nextItemId++, "Modern Cataloguing", "I. Librarian", 2017, "9999999999", "020.50"));
    m_catalogue.addItem(m_factory.createNonFictionBook(
        m_nextItemId++, "User Experience in Libraries", "J. Designer", 2019, "1010101010", "025.56"));

    // 3 magazines
    m_catalogue.addItem(m_factory.createMagazine(
        m_nextItemId++, "Tech Monthly", "Tech Media", 2024, "2024202401", "Issue 1", QDate(2024, 1, 15)));
    m_catalogue.addItem(m_factory.createMagazine(
        m_nextItemId++, "Library World", "Lib Press", 2024, "2024202402", "Issue 5", QDate(2024, 5, 1)));
    m_catalogue.addItem(m_factory.createMagazine(
        m_nextItemId++, "Game Review", "Games Inc.", 2023, "2023202309", "Issue 9", QDate(2023, 9, 10)));

    // 3 movies
    m_catalogue.addItem(m_factory.createMovie(
        m_nextItemId++, "Stacks of Destiny", "K. Director", 2016, "Drama", "PG"));
    m_catalogue.addItem(m_factory.createMovie(
        m_nextItemId++, "Night at HinLIBS", "L. Director", 2018, "Comedy", "PG-13"));
    m_catalogue.addItem(m_factory.createMovie(
        m_nextItemId++, "Return Policy", "M. Director", 2020, "Thriller", "R"));

    // 4 video games
    m_catalogue.addItem(m_factory.createVideoGame(
        m_nextItemId++, "Catalogue Quest", "Studio X", 2021, "RPG", "T"));
    m_catalogue.addItem(m_factory.createVideoGame(
        m_nextItemId++, "Overdue Odyssey", "Studio Y", 2022, "Adventure", "E10+"));
    m_catalogue.addItem(m_factory.createVideoGame(
        m_nextItemId++, "Hold Line Heroes", "Studio Z", 2023, "Strategy", "T"));
    m_catalogue.addItem(m_factory.createVideoGame(
        m_nextItemId++, "Shelving Simulator", "Studio Q", 2019, "Simulation", "E"));

    // Users: 5 patrons, 1 librarian, 1 admin
    m_users.emplace_back(m_nextUserId++, "alice", UserRole::Patron);
    m_users.emplace_back(m_nextUserId++, "bob", UserRole::Patron);
    m_users.emplace_back(m_nextUserId++, "carol", UserRole::Patron);
    m_users.emplace_back(m_nextUserId++, "dave", UserRole::Patron);
    m_users.emplace_back(m_nextUserId++, "erin", UserRole::Patron);

    m_users.emplace_back(m_nextUserId++, "libby", UserRole::Librarian);
    m_users.emplace_back(m_nextUserId++, "admin", UserRole::Administrator);
}

// Look up a user by name (case-insensitive).
FindUserResult HinLIBSSystem::findUserByName(const QString &name) const
{
    for (const auto &u : m_users) {
        if (u.name().compare(name, Qt::CaseInsensitive) == 0) {
            return { true, u.id(), u.role() };
        }
    }
    return { false, -1, UserRole::Patron };
}

// Build a view of the catalogue for display in the UI.
std::vector<BrowseItemInfo> HinLIBSSystem::browseCatalogue() const
{
    std::vector<BrowseItemInfo> out;
    for (const auto &ptr : m_catalogue.items()) {
        QString statusText = (ptr->status() == ItemStatus::Available)
                ? "Available" : "Checked Out";
        out.push_back({ ptr->id(), ptr->title(), ptr->creator(),
                        ptr->formatName(), statusText });
    }
    return out;
}

// Compute full account status for a patron: active loans and holds.
PatronAccountStatus HinLIBSSystem::getPatronAccountStatus(int patronId) const
{
    PatronAccountStatus status;
    const User *patron = findUserById(patronId);
    //If id is invalid or not a patron, return an empty status.
    if (!patron || patron->role() != UserRole::Patron)
        return status;

    QDate today = QDate::currentDate();

    // Convert each active loan id into LoanInfo with title, due date, and days remaining.
    for (int itemId : patron->activeLoans()) {
        const CatalogueItem *item = m_catalogue.findItemById(itemId);
        if (!item) continue;
        int daysRemaining = today.daysTo(item->dueDate());
        status.loans.push_back({
            item->id(),
            item->title(),
            item->dueDate().toString("yyyy-MM-dd"),
            daysRemaining
        });
    }

    // Convert each active hold id into HoldInfo with title and current FIFO queue position.
    for (int itemId : patron->activeHolds()) {
        const CatalogueItem *item = m_catalogue.findItemById(itemId);
        if (!item) continue;
        int pos = findHoldPosition(item, patronId);
        status.holds.push_back({
            item->id(),
            item->title(),
            pos
        });
    }

    return status;
}

// Librarian-only: add a new catalogue item, updating SQLite and in-memory model.
OperationResult HinLIBSSystem::addCatalogueItem(const QString &title,
                                                const QString &creator,
                                                int year,
                                                const QString &isbn,
                                                const QString &typeStr,
                                                const QString &dewey,
                                                const QString &issueNumber,
                                                const QDate &pubDate,
                                                const QString &genre,
                                                const QString &rating)
{
    if (title.trimmed().isEmpty() || creator.trimmed().isEmpty()) {
        return { false, "Title and creator are required." };
    }

    int newId = m_nextItemId++;

    // 1) Insert into SQLite
    QSqlDatabase db = m_dbManager->database();
    QSqlQuery q(db);
    q.prepare("INSERT INTO catalogue "
              "(id, title, creator, year, isbn, type, dewey, issueNumber, pubDate, genre, rating, status) "
              "VALUES (?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, 'Available')");
    q.addBindValue(newId);
    q.addBindValue(title);
    q.addBindValue(creator);
    q.addBindValue(year);
    q.addBindValue(isbn);
    q.addBindValue(typeStr);
    q.addBindValue(dewey);
    q.addBindValue(issueNumber);
    q.addBindValue(pubDate.isValid() ? pubDate.toString("yyyy-MM-dd") : QString());
    q.addBindValue(genre);
    q.addBindValue(rating);

    if (!q.exec()) {
        return { false, "Failed to insert into database."};
    }

    // 2) Create in-memory item using Factory and add to catalogue
    std::unique_ptr<CatalogueItem> item;

    if (typeStr == "FictionBook") {
        item = m_factory.createFictionBook(newId, title, creator, year, isbn);
    } else if (typeStr == "NonFictionBook") {
        item = m_factory.createNonFictionBook(newId, title, creator, year, isbn, dewey);
    } else if (typeStr == "Magazine") {
        item = m_factory.createMagazine(newId, title, creator, year, isbn,
                                        issueNumber, pubDate);
    } else if (typeStr == "Movie") {
        item = m_factory.createMovie(newId, title, creator, year, genre, rating);
    } else if (typeStr == "VideoGame") {
        item = m_factory.createVideoGame(newId, title, creator, year, genre, rating);
    } else {
        return { false, "Unknown item type."};
    }

    m_catalogue.addItem(std::move(item));

    return { true, "Item added to catalogue."};
}

OperationResult HinLIBSSystem::removeCatalogueItem(int itemId)
{
    CatalogueItem *item = m_catalogue.findItemById(itemId);
    if (!item)
        return { false, "Item not found." };

    if (item->status() == ItemStatus::CheckedOut)
        return { false, "Cannot remove an item that is currently checked out."};

    if (!item->holdQueue().empty())
        return { false, "Cannot remove an item that has active holds."};

    // Delete from DB
    QSqlDatabase db = m_dbManager->database();
    QSqlQuery q(db);
    q.prepare("DELETE FROM catalogue WHERE id = ?");
    q.addBindValue(itemId);
    if (!q.exec()) {
        return { false, "Failed to delete from database." };
    }

    // Delete from in-memory catalogue
    auto &vec = m_catalogue.items();
    vec.erase(std::remove_if(vec.begin(), vec.end(),
                             [itemId](const std::unique_ptr<CatalogueItem> &p) {
                                 return p->id() == itemId;
                             }),
              vec.end());

    return { true, "Item removed from catalogue."};
}

// Find a patron by name, returning their id through patronIdOut.
OperationResult HinLIBSSystem::findPatronByName(const QString &name, int &patronIdOut)
{
    for (const auto &u : m_users) {
        if (u.role() == UserRole::Patron &&
            u.name().compare(name, Qt::CaseInsensitive) == 0) {
            patronIdOut = u.id();
            return { true, "Patron found." };
        }
    }
    return { false, "Patron not found." };


}

// Retrieve all active loans for a given patron from the loans table.
std::vector<PatronLoanInfo> HinLIBSSystem::getPatronLoans(int patronId) const
{
    std::vector<PatronLoanInfo> out;
    const User *patron = findUserById(patronId);
    if (!patron || patron->role() != UserRole::Patron)
        return out;

    QSqlDatabase db = m_dbManager->database();
    QSqlQuery q(db);
    q.prepare("SELECT itemId, checkoutDate, dueDate FROM loans WHERE patronId = ?");
    q.addBindValue(patronId);
    if (!q.exec()) {
        return out;
    }

    while (q.next()) {
        int itemId = q.value(0).toInt();
        QString checkoutDate = q.value(1).toString();
        QString dueDate = q.value(2).toString();

        const CatalogueItem *item = m_catalogue.findItemById(itemId);
        QString title = item ? item->title() : QString("Unknown");

        out.push_back({ itemId, title, checkoutDate, dueDate });
    }

    return out;
}

// This is used by the librarian feature to reuse the normal returnItem logic.
OperationResult HinLIBSSystem::returnItemForPatron(int patronId, int itemId)
{
    // Reuse the same logic as patron return.
    auto result = returnItem(patronId, itemId);
    return result;
}

// Borrow an item as a patron: checks rules, updates memory and SQLite.
OperationResult HinLIBSSystem::borrowItem(int patronId, int itemId)
{
    // Enforce that the user exists and is a patron.
    User *patron = findUserById(patronId);
    if (!patron || patron->role() != UserRole::Patron)
        return { false, "Invalid patron." };

    // Enforce that the item exists and is currently available.
    CatalogueItem *item = m_catalogue.findItemById(itemId);
    if (!item)
        return { false, "Item not found." };

    if (item->status() != ItemStatus::Available)
        return { false, "Item is not available." };

    if (static_cast<int>(patron->activeLoans().size()) >= 3)  // max 3 loans
        return { false, "Maximum of 3 active loans reached." };

    // Update item
    item->setStatus(ItemStatus::CheckedOut);
    item->setCurrentBorrowerId(patronId);
    item->setDueDate(QDate::currentDate().addDays(14));       // 14 days

    // Update patron
    patron->activeLoans().push_back(itemId);

    QSqlDatabase db = m_dbManager->database();

    // Insert new loan row
    QSqlQuery q(db);
    q.prepare("INSERT INTO loans (patronId, itemId, checkoutDate, dueDate) "
              "VALUES (?, ?, ?, ?)");
    q.addBindValue(patronId);
    q.addBindValue(itemId);
    q.addBindValue(QDate::currentDate().toString("yyyy-MM-dd"));
    q.addBindValue(item->dueDate().toString("yyyy-MM-dd"));
    if (!q.exec()) {
        qWarning() << "Failed to insert loan:" << q.lastError().text();
    }

    // Update catalogue status in DB
    QSqlQuery q2(db);
    q2.prepare("UPDATE catalogue SET status = 'CheckedOut' WHERE id = ?");
    q2.addBindValue(itemId);
    if (!q2.exec()) {
        qWarning() << "Failed to update item status:" << q2.lastError().text();
    }


    return { true, "Borrow successful." };
}

// Return an item as a patron (or via librarian), updating memory and SQLite.
OperationResult HinLIBSSystem::returnItem(int patronId, int itemId)
{
    // Validate patron and item, and ensure the patron actually has this loan.
    User *patron = findUserById(patronId);
    if (!patron || patron->role() != UserRole::Patron)
        return { false, "Invalid patron." };

    CatalogueItem *item = m_catalogue.findItemById(itemId);
    if (!item)
        return { false, "Item not found." };

    // Remove from patron loans
    auto &loans = patron->activeLoans();
    auto it = std::find(loans.begin(), loans.end(), itemId);
    if (it == loans.end())
        return { false, "Item is not currently borrowed by this patron." };

    loans.erase(it);

    // Clear item loan data
    item->setStatus(ItemStatus::Available);
    item->setCurrentBorrowerId(-1);
    item->setDueDate(QDate());

    // If there is a hold queue, next in queue would be notified in a full system;

    QSqlDatabase db = m_dbManager->database();

    // Delete loan row
    QSqlQuery q(db);
    q.prepare("DELETE FROM loans WHERE patronId = ? AND itemId = ?");
    q.addBindValue(patronId);
    q.addBindValue(itemId);
    if (!q.exec()) {
        qWarning() << "Failed to delete loan:" << q.lastError().text();
    }

    // Update catalogue status back to Available
    QSqlQuery q2(db);
    q2.prepare("UPDATE catalogue SET status = 'Available' WHERE id = ?");
    q2.addBindValue(itemId);
    if (!q2.exec()) {
        qWarning() << "Failed to update item status:" << q2.lastError().text();
    }

    return { true, "Return successful." };
}

// Place a hold on a checked-out item, updating only in-memory structures for D2.
OperationResult HinLIBSSystem::placeHold(int patronId, int itemId)
{
    // Validate patron and item.
    User *patron = findUserById(patronId);
    if (!patron || patron->role() != UserRole::Patron)
        return { false, "Invalid patron." };

    CatalogueItem *item = m_catalogue.findItemById(itemId);
    if (!item)
        return { false, "Item not found." };

    if (item->status() == ItemStatus::Available)
        return { false, "Holds allowed only on checked-out items." };

    // Check not already in queue
    auto &queue = item->holdQueue();
    if (std::find(queue.begin(), queue.end(), patronId) != queue.end())
        return { false, "Patron already has a hold for this item." };

    queue.push_back(patronId); // FIFO [file:2]

    patron->activeHolds().push_back(itemId);

    return { true, "Hold placed." };
}

// Cancel a hold by removing the patron from both the item queue and patron list.
OperationResult HinLIBSSystem::cancelHold(int patronId, int itemId)
{
    User *patron = findUserById(patronId);
    if (!patron || patron->role() != UserRole::Patron)
        return { false, "Invalid patron." };

    CatalogueItem *item = m_catalogue.findItemById(itemId);
    if (!item)
        return { false, "Item not found." };

    auto &queue = item->holdQueue();
    auto it = std::find(queue.begin(), queue.end(), patronId);
    if (it == queue.end())
        return { false, "Hold not found for this patron." };

    queue.erase(it);

    auto &holds = patron->activeHolds();
    auto it2 = std::find(holds.begin(), holds.end(), itemId);
    if (it2 != holds.end())
        holds.erase(it2);

    return { true, "Hold cancelled." };
}

// Helper: find a user by id (const version).
const User *HinLIBSSystem::findUserById(int id) const
{
    for (const auto &u : m_users) {
        if (u.id() == id) return &u;
    }
    return nullptr;
}

// Helper: find a user by id (non-const version).
User *HinLIBSSystem::findUserById(int id)
{
    for (auto &u : m_users) {
        if (u.id() == id) return &u;
    }
    return nullptr;
}

// Compute a patron's 1-based position in an item's hold queue.
int HinLIBSSystem::findHoldPosition(const CatalogueItem *item, int patronId) const
{
    // Return the 1-based position of patronId in the item’s holdQueue (FIFO), or -1 if not present.
    const auto &queue = item->holdQueue();
    int pos = 1;
    for (int pid : queue) {
        if (pid == patronId)
            return pos;
        ++pos;
    }
    return -1;
}

// Load all catalogue, user, loan, and hold data from SQLite into memory at startup.
void HinLIBSSystem::loadFromDatabase()
{
    m_catalogue.items().clear();
    m_users.clear();

    QSqlDatabase db = m_dbManager->database();

    // 1) Load catalogue rows and build CatalogueItem objects
    QSqlQuery itemQuery(db);
    if (!itemQuery.exec("SELECT id, title, creator, year, isbn, type, dewey, "
                        "issueNumber, pubDate, genre, rating, status FROM catalogue")) {
        qWarning() << "Failed to load catalogue:" << itemQuery.lastError().text();
        return;
    }

    while (itemQuery.next()) {
        int id = itemQuery.value(0).toInt();
        QString title = itemQuery.value(1).toString();
        QString creator = itemQuery.value(2).toString();
        int year = itemQuery.value(3).toInt();
        QString isbn = itemQuery.value(4).toString();
        QString typeStr = itemQuery.value(5).toString();
        QString dewey = itemQuery.value(6).toString();
        QString issue = itemQuery.value(7).toString();
        QString pubDateStr = itemQuery.value(8).toString();
        QString genre = itemQuery.value(9).toString();
        QString rating = itemQuery.value(10).toString();
        QString statusStr = itemQuery.value(11).toString();

        std::unique_ptr<CatalogueItem> item;

        if (typeStr == "FictionBook") {
            item = m_factory.createFictionBook(id, title, creator, year, isbn);
        } else if (typeStr == "NonFictionBook") {
            item = m_factory.createNonFictionBook(id, title, creator, year, isbn, dewey);
        } else if (typeStr == "Magazine") {
            QDate pubDate = QDate::fromString(pubDateStr, "yyyy-MM-dd");
            item = m_factory.createMagazine(id, title, creator, year, isbn, issue, pubDate);
        } else if (typeStr == "Movie") {
            item = m_factory.createMovie(id, title, creator, year, genre, rating);
        } else if (typeStr == "VideoGame") {
            item = m_factory.createVideoGame(id, title, creator, year, genre, rating);
        } else {
            continue;
        }

        if (statusStr == "CheckedOut")
            item->setStatus(ItemStatus::CheckedOut);
        else
            item->setStatus(ItemStatus::Available);

        m_catalogue.addItem(std::move(item));
        if (id >= m_nextItemId)
            m_nextItemId = id + 1;
    }

    // 2) Load users
    QSqlQuery userQuery(db);
    if (!userQuery.exec("SELECT id, name, role FROM users")) {
        qWarning() << "Failed to load users:" << userQuery.lastError().text();
        return;
    }

    while (userQuery.next()) {
        int id = userQuery.value(0).toInt();
        QString name = userQuery.value(1).toString();
        QString roleStr = userQuery.value(2).toString();

        UserRole role = UserRole::Patron;
        if (roleStr == "Librarian") role = UserRole::Librarian;
        else if (roleStr == "Administrator") role = UserRole::Administrator;

        m_users.emplace_back(id, name, role);
        if (id >= m_nextUserId)
            m_nextUserId = id + 1;
    }

    // 3) Load loans
    QSqlQuery loanQuery(db);
    if (!loanQuery.exec("SELECT patronId, itemId, dueDate FROM loans")) {
        qWarning() << "Failed to load loans:" << loanQuery.lastError().text();
        return;
    }

    while (loanQuery.next()) {
        int patronId = loanQuery.value(0).toInt();
        int itemId = loanQuery.value(1).toInt();
        QString dueDateStr = loanQuery.value(2).toString();

        User *patron = findUserById(patronId);
        CatalogueItem *item = m_catalogue.findItemById(itemId);
        if (!patron || !item) continue;

        patron->activeLoans().push_back(itemId);
        item->setStatus(ItemStatus::CheckedOut);
        item->setCurrentBorrowerId(patronId);
        item->setDueDate(QDate::fromString(dueDateStr, "yyyy-MM-dd"));
    }

    // 4) Load holds (ordered by item and position)
    QSqlQuery holdQuery(db);
    if (!holdQuery.exec("SELECT patronId, itemId, position FROM holds "
                        "ORDER BY itemId, position")) {
        qWarning() << "Failed to load holds:" << holdQuery.lastError().text();
        return;
    }

    while (holdQuery.next()) {
        int patronId = holdQuery.value(0).toInt();
        int itemId = holdQuery.value(1).toInt();

        User *patron = findUserById(patronId);
        CatalogueItem *item = m_catalogue.findItemById(itemId);
        if (!patron || !item) continue;

        item->holdQueue().push_back(patronId);
        patron->activeHolds().push_back(itemId);
    }
}


