#include "patronwindow.h"
#include "hinlibssystem.h"

#include <QTableWidget>
#include <QHeaderView>
#include <QPushButton>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QMessageBox>
#include <QWidget>

PatronWindow::PatronWindow(HinLIBSSystem *system, int patronId, QWidget *parent)
    : QMainWindow(parent),
      m_system(system),// Pointer to shared application logic.
      m_patronId(patronId) // The logged-in patron's id.
{
    setupUi();
    loadCatalogue();// Fill catalogue table.
    loadAccountStatus(); // Fill loans/holds tables and summary label.
    setWindowTitle("HinLIBS - Patron");
}

void PatronWindow::setupUi()
{
    QWidget *central = new QWidget(this);
    QVBoxLayout *mainLayout = new QVBoxLayout(central);

    // Catalogue table setup (shows all items)
    QLabel *catLabel = new QLabel("Catalogue:", central);
    m_catalogueTable = new QTableWidget(0, 5, central);
    m_catalogueTable->setHorizontalHeaderLabels(
        QStringList() << "ID" << "Title" << "Creator" << "Format" << "Status");
    m_catalogueTable->horizontalHeader()->setStretchLastSection(true);
    m_catalogueTable->setSelectionBehavior(QAbstractItemView::SelectRows);
    m_catalogueTable->setSelectionMode(QAbstractItemView::SingleSelection);
    m_catalogueTable->setEditTriggers(QAbstractItemView::NoEditTriggers);

    // Loans table: ID, Title, Due Date, Days Remaining.
    QLabel *loansLabel = new QLabel("Active loans:", central);
    m_loansTable = new QTableWidget(0, 4, central);
    m_loansTable->setHorizontalHeaderLabels(
        QStringList() << "ID" << "Title" << "Due Date" << "Days Remaining");
    m_loansTable->horizontalHeader()->setStretchLastSection(true);
    m_loansTable->setSelectionBehavior(QAbstractItemView::SelectRows);
    m_loansTable->setSelectionMode(QAbstractItemView::SingleSelection);
    m_loansTable->setEditTriggers(QAbstractItemView::NoEditTriggers);

    // Holds table: ID, Title, Queue Position (FIFO).
    QLabel *holdsLabel = new QLabel("Active holds:", central);
    m_holdsTable = new QTableWidget(0, 3, central);
    m_holdsTable->setHorizontalHeaderLabels(
        QStringList() << "ID" << "Title" << "Queue Position");
    m_holdsTable->horizontalHeader()->setStretchLastSection(true);
    m_holdsTable->setSelectionBehavior(QAbstractItemView::SelectRows);
    m_holdsTable->setSelectionMode(QAbstractItemView::SingleSelection);
    m_holdsTable->setEditTriggers(QAbstractItemView::NoEditTriggers);

    m_summaryLabel = new QLabel("", central); // Shows counts of loans/holds.

    // Action buttons for patron operations.
    m_borrowButton = new QPushButton("Borrow selected", central);
    m_placeHoldButton = new QPushButton("Place hold on selected", central);
    m_returnButton = new QPushButton("Return selected loan", central);
    m_cancelHoldButton = new QPushButton("Cancel selected hold", central);
    m_refreshButton = new QPushButton("Refresh", central);
    m_logoutButton = new QPushButton("Logout", central);
    m_viewAccountStatusButton = new QPushButton("View account status", central);


    // Layouts grouping buttons near relevant tables.
    QHBoxLayout *topButtons = new QHBoxLayout;
    topButtons->addWidget(m_borrowButton);
    topButtons->addWidget(m_placeHoldButton);

    QHBoxLayout *midButtons = new QHBoxLayout;
    midButtons->addWidget(m_returnButton);
    midButtons->addWidget(m_cancelHoldButton);

    QHBoxLayout *bottomButtons = new QHBoxLayout;
    bottomButtons->addWidget(m_viewAccountStatusButton); // Pops up summary dialog.
    bottomButtons->addWidget(m_refreshButton);// Refreshes tables in-place.
    bottomButtons->addStretch();
    bottomButtons->addWidget(m_logoutButton);

    // Assemble UI.
    mainLayout->addWidget(catLabel);
    mainLayout->addWidget(m_catalogueTable);
    mainLayout->addLayout(topButtons);

    mainLayout->addWidget(loansLabel);
    mainLayout->addWidget(m_loansTable);
    mainLayout->addLayout(midButtons);

    mainLayout->addWidget(holdsLabel);
    mainLayout->addWidget(m_holdsTable);
    mainLayout->addWidget(m_summaryLabel);
    mainLayout->addLayout(bottomButtons);

    setCentralWidget(central);

    // Connect buttons to slots that call HinLIBSSystem operations.
    connect(m_borrowButton, &QPushButton::clicked,
            this, &PatronWindow::onBorrowClicked);
    connect(m_returnButton, &QPushButton::clicked,
            this, &PatronWindow::onReturnClicked);
    connect(m_placeHoldButton, &QPushButton::clicked,
            this, &PatronWindow::onPlaceHoldClicked);
    connect(m_cancelHoldButton, &QPushButton::clicked,
            this, &PatronWindow::onCancelHoldClicked);
    connect(m_refreshButton, &QPushButton::clicked,
            this, &PatronWindow::onRefreshClicked);
    connect(m_logoutButton, &QPushButton::clicked,
            this, &PatronWindow::onLogoutClicked);
    connect(m_viewAccountStatusButton, &QPushButton::clicked,   // NEW
            this, &PatronWindow::onViewAccountStatusClicked);
}

void PatronWindow::loadCatalogue()
{
    // Ask HinLIBSSystem for a list of all items and render into the catalogue table.
    auto list = m_system->browseCatalogue();
    m_catalogueTable->setRowCount(static_cast<int>(list.size()));
    int row = 0;
    for (const auto &info : list) {
        m_catalogueTable->setItem(row, 0, new QTableWidgetItem(QString::number(info.id)));
        m_catalogueTable->setItem(row, 1, new QTableWidgetItem(info.title));
        m_catalogueTable->setItem(row, 2, new QTableWidgetItem(info.creator));
        m_catalogueTable->setItem(row, 3, new QTableWidgetItem(info.format));
        m_catalogueTable->setItem(row, 4, new QTableWidgetItem(info.statusText));
        ++row;
    }
}

void PatronWindow::loadAccountStatus()
{

    // Query the system for the patron's current loans and holds.
    auto status = m_system->getPatronAccountStatus(m_patronId);

    // Populate loans table.
    m_loansTable->setRowCount(static_cast<int>(status.loans.size()));
    int row = 0;
    for (const auto &loan : status.loans) {
        m_loansTable->setItem(row, 0, new QTableWidgetItem(QString::number(loan.itemId)));
        m_loansTable->setItem(row, 1, new QTableWidgetItem(loan.title));
        m_loansTable->setItem(row, 2, new QTableWidgetItem(loan.dueDateString));
        m_loansTable->setItem(row, 3, new QTableWidgetItem(QString::number(loan.daysRemaining)));
        ++row;
    }

    m_holdsTable->setRowCount(static_cast<int>(status.holds.size()));
    row = 0;
    for (const auto &hold : status.holds) {
        m_holdsTable->setItem(row, 0, new QTableWidgetItem(QString::number(hold.itemId)));
        m_holdsTable->setItem(row, 1, new QTableWidgetItem(hold.title));
        m_holdsTable->setItem(row, 2, new QTableWidgetItem(QString::number(hold.queuePosition)));
        ++row;
    }

    // Brief summary label below the tables.
    m_summaryLabel->setText(
        QString("Active loans: %1 (max 3), Active holds: %2")
            .arg(status.loans.size())
            .arg(status.holds.size()));
}

// Helper: get the item id from the selected row in the catalogue table.
int PatronWindow::selectedCatalogueItemId() const
{
    auto indexes = m_catalogueTable->selectionModel()->selectedRows();
    if (indexes.isEmpty()) return -1;
    bool ok = false;
    int id = m_catalogueTable->item(indexes.first().row(), 0)->text().toInt(&ok);
    return ok ? id : -1;
}

// Helper: get the item id for the selected loan row.
int PatronWindow::selectedLoanItemId() const
{
    auto indexes = m_loansTable->selectionModel()->selectedRows();
    if (indexes.isEmpty()) return -1;
    bool ok = false;
    int id = m_loansTable->item(indexes.first().row(), 0)->text().toInt(&ok);
    return ok ? id : -1;
}

// Helper: get the item id for the selected hold row.
int PatronWindow::selectedHoldItemId() const
{
    auto indexes = m_holdsTable->selectionModel()->selectedRows();
    if (indexes.isEmpty()) return -1;
    bool ok = false;
    int id = m_holdsTable->item(indexes.first().row(), 0)->text().toInt(&ok);
    return ok ? id : -1;
}

// Slot: borrow the currently selected catalogue item for this patron.
void PatronWindow::onBorrowClicked()
{
    int itemId = selectedCatalogueItemId();
    if (itemId < 0) {
        QMessageBox::warning(this, "Borrow", "Select an item.");
        return;
    }

    auto result = m_system->borrowItem(m_patronId, itemId);
    QMessageBox::information(this, "Borrow",
                             result.success ? result.message : result.message);
    loadCatalogue();
    loadAccountStatus();
}

// Slot: return the currently selected loan.
void PatronWindow::onReturnClicked()
{
    int itemId = selectedLoanItemId();
    if (itemId < 0) {
        QMessageBox::warning(this, "Return", "Select a loan.");
        return;
    }

    auto result = m_system->returnItem(m_patronId, itemId);
    QMessageBox::information(this, "Return",
                             result.success ? result.message : result.message);
    loadCatalogue();
    loadAccountStatus();
}

// Slot: place a hold on the selected catalogue item.
void PatronWindow::onPlaceHoldClicked()
{
    int itemId = selectedCatalogueItemId();
    if (itemId < 0) {
        QMessageBox::warning(this, "Hold", "Select an item.");
        return;
    }

    auto result = m_system->placeHold(m_patronId, itemId);
    QMessageBox::information(this, "Hold",
                             result.success ? result.message : result.message);
    loadCatalogue();
    loadAccountStatus();
}

// Slot: cancel the selected hold.
void PatronWindow::onCancelHoldClicked()
{
    int itemId = selectedHoldItemId();
    if (itemId < 0) {
        QMessageBox::warning(this, "Cancel Hold", "Select a hold.");
        return;
    }

    auto result = m_system->cancelHold(m_patronId, itemId);
    QMessageBox::information(this, "Cancel Hold",
                             result.success ? result.message : result.message);
    loadCatalogue();
    loadAccountStatus();
}

// Slot: refresh both the catalogue view and account status.
void PatronWindow::onRefreshClicked()
{
    loadCatalogue();
    loadAccountStatus();
}

// Slot: logout from patron view and return to the login window.
void PatronWindow::onLogoutClicked()
{
    emit logoutRequested();
    close();
}
void PatronWindow::onViewAccountStatusClicked()
{
    auto status = m_system->getPatronAccountStatus(m_patronId);

    QString text;
    text += "Active loans:\n";
    if (status.loans.empty()) {
        text += "  (none)\n";
    } else {
        for (const auto &loan : status.loans) {
            text += QString("  • %1 | Due: %2 | Days remaining: %3\n")
                        .arg(loan.title)
                        .arg(loan.dueDateString)
                        .arg(loan.daysRemaining);
        }
    }

    text += "\nActive holds:\n";
    if (status.holds.empty()) {
        text += "  (none)\n";
    } else {
        for (const auto &hold : status.holds) {
            text += QString("  • %1 | Queue position: %2\n")
                        .arg(hold.title)
                        .arg(hold.queuePosition);
        }
    }

    QMessageBox::information(this, "Account status", text);
}


