#include "librarianwindow.h"
#include "hinlibssystem.h"
#include <QVBoxLayout>
#include <QLabel>
#include <QPushButton>
#include <QWidget>
#include <QInputDialog>
#include <QMessageBox>
#include <QDate>


// Librarian main window: provides access to librarian-only functions
// such as adding/removing items and returning items for patrons.
LibrarianWindow::LibrarianWindow(HinLIBSSystem *system, int, QWidget *parent)
    : QMainWindow(parent),
      m_system(system)
{
    QWidget *central = new QWidget(this);
        QVBoxLayout *layout = new QVBoxLayout(central);

        auto *addBtn = new QPushButton("Add item to catalogue", central);
        auto *removeBtn = new QPushButton("Remove item from catalogue", central);
        auto *returnBtn = new QPushButton("Return item for patron", central);
        auto *logoutBtn = new QPushButton("Logout", central);

        layout->addWidget(addBtn);
        layout->addWidget(removeBtn);
        layout->addWidget(returnBtn);
        layout->addStretch();
        layout->addWidget(logoutBtn);

        setCentralWidget(central);
        setWindowTitle("HinLIBS - Librarian");

        // Wire up buttons to the corresponding slots.
        connect(addBtn, &QPushButton::clicked, this, &LibrarianWindow::onAddItemClicked);
        connect(removeBtn, &QPushButton::clicked, this, &LibrarianWindow::onRemoveItemClicked);
        connect(returnBtn, &QPushButton::clicked, this, &LibrarianWindow::onReturnForPatronClicked);
        connect(logoutBtn, &QPushButton::clicked, [this]() {
            emit logoutRequested();
            close();
        });
}

// Collects item details from the librarian and delegates creation to HinLIBSSystem.
void LibrarianWindow::onAddItemClicked()
{
    // Ask for basic fields using simple dialogs.
    bool ok = false;

    QString title = QInputDialog::getText(this, "Add item",
                                          "Title:", QLineEdit::Normal,
                                          "", &ok);
    if (!ok || title.trimmed().isEmpty())
        return;

    QString creator = QInputDialog::getText(this, "Add item",
                                            "Author / Creator:", QLineEdit::Normal,
                                            "", &ok);
    if (!ok || creator.trimmed().isEmpty())
        return;

    int year = QInputDialog::getInt(this, "Add item",
                                    "Publication year:", 2024, 0, 3000, 1, &ok);
    if (!ok)
        return;

    QString isbn = QInputDialog::getText(this, "Add item",
                                         "ISBN (leave empty if N/A):", QLineEdit::Normal,
                                         "", &ok);
    if (!ok)
        return;

    // Choose type as a simple string that matches your DB and factory.
    QStringList types{ "FictionBook", "NonFictionBook", "Magazine", "Movie", "VideoGame" };
    QString typeStr = QInputDialog::getItem(this, "Add item",
                                            "Type:", types, 0, false, &ok);
    if (!ok || typeStr.isEmpty())
        return;

    QString dewey;
    QString issueNumber;
    QDate pubDate;
    QString genre;
    QString rating;

    if (typeStr == "NonFictionBook") {
        dewey = QInputDialog::getText(this, "Add item",
                                      "Dewey (XXX.XX):", QLineEdit::Normal,
                                      "", &ok);
        if (!ok)
            return;
    } else if (typeStr == "Magazine") {
        issueNumber = QInputDialog::getText(this, "Add item",
                                            "Issue number:", QLineEdit::Normal,
                                            "", &ok);
        if (!ok)
            return;

        QString pubDateStr = QInputDialog::getText(this, "Add item",
                                                   "Publication date (YYYY-MM-DD):",
                                                   QLineEdit::Normal,
                                                   "2024-01-01", &ok);
        if (!ok)
            return;
        pubDate = QDate::fromString(pubDateStr, "yyyy-MM-dd");
    } else if (typeStr == "Movie" || typeStr == "VideoGame") {
        genre = QInputDialog::getText(this, "Add item",
                                      "Genre:", QLineEdit::Normal,
                                      "", &ok);
        if (!ok)
            return;
        rating = QInputDialog::getText(this, "Add item",
                                       "Rating:", QLineEdit::Normal,
                                       "", &ok);
        if (!ok)
            return;
    }

    // Call HinLIBSSystem to actually create the item (DB + in-memory).
    auto result = m_system->addCatalogueItem(title,
                                             creator,
                                             year,
                                             isbn,
                                             typeStr,
                                             dewey,
                                             issueNumber,
                                             pubDate,
                                             genre,
                                             rating);

    QMessageBox::information(this, "Add item", result.message);
}

// Prompt for an item id and request its removal from the system.
void LibrarianWindow::onRemoveItemClicked()
{
    bool ok = false;
    int id = QInputDialog::getInt(this, "Remove item",
                                  "Item ID:", 1, 1, 1000000, 1, &ok);
    if (!ok)
        return;

    auto result = m_system->removeCatalogueItem(id);
    QMessageBox::information(this, "Remove item", result.message);
}

// Search for a patron by name, show their active loans, and return a chosen item.
void LibrarianWindow::onReturnForPatronClicked()
{
    bool ok = false;
    QString name = QInputDialog::getText(this, "Find patron",
                                         "Patron name:", QLineEdit::Normal,
                                         "", &ok);
    if (!ok || name.trimmed().isEmpty())
        return;

    int patronId = -1;
    auto findResult = m_system->findPatronByName(name.trimmed(), patronId);
    if (!findResult.success) {
        QMessageBox::warning(this, "Patron", findResult.message);
        return;
    }

    // Fetch active loans for the selected patron from the system/DB.
    auto loans = m_system->getPatronLoans(patronId);
    if (loans.empty()) {
        QMessageBox::information(this, "Loans", "This patron has no active loans.");
        return;
    }

    // Build a simple list string for the librarian to choose by item id.
    QString listText;
    for (const auto &loan : loans) {
        listText += QString("ID %1: %2 (Due %3)\n")
                        .arg(loan.itemId)
                        .arg(loan.title)
                        .arg(loan.dueDate);
    }
    QMessageBox::information(this, "Active loans", listText);

    int itemId = QInputDialog::getInt(this, "Return item",
                                      "Enter item ID to return:", loans[0].itemId,
                                      1, 1000000, 1, &ok);
    if (!ok)
        return;

    auto result = m_system->returnItemForPatron(patronId, itemId);
    QMessageBox::information(this, "Return", result.message);
}

