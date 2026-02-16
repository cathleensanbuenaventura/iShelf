#ifndef PATRONWINDOW_H
#define PATRONWINDOW_H

#include <QMainWindow>

class QTableWidget;
class QPushButton;
class QLabel;
class HinLIBSSystem;

// Main UI for a patron user.
// Shows the catalogue, the patron's loans and holds, and allows them
// to borrow/return items and manage holds.
class PatronWindow : public QMainWindow
{
    Q_OBJECT

public:
    // system: shared backend; patronId: id of the logged-in patron.
    explicit PatronWindow(HinLIBSSystem *system, int patronId, QWidget *parent = nullptr);

signals:
    void logoutRequested();

private slots:
    // Button handlers for each patron action.
    void onBorrowClicked();
    void onReturnClicked();
    void onPlaceHoldClicked();
    void onCancelHoldClicked();
    void onRefreshClicked();
    void onLogoutClicked();
    void onViewAccountStatusClicked();

private:
    HinLIBSSystem *m_system; // Pointer to core system logic.
    int m_patronId; // Currently logged-in patron's user id.

    // Tables for viewing catalogue, active loans, and active holds.
    QTableWidget *m_catalogueTable;
    QTableWidget *m_loansTable;
    QTableWidget *m_holdsTable;
    QLabel *m_summaryLabel;

    // Action buttons in the patron UI.
    QPushButton *m_borrowButton;
    QPushButton *m_returnButton;
    QPushButton *m_placeHoldButton;
    QPushButton *m_cancelHoldButton;
    QPushButton *m_refreshButton;
    QPushButton *m_logoutButton;
    QPushButton *m_viewAccountStatusButton;

    // Helper functions to construct the UI and keep it in sync with the model.
    void setupUi();
    void loadCatalogue();
    void loadAccountStatus();

    // Helpers to fetch the selected item id from each table.
    int selectedCatalogueItemId() const;
    int selectedLoanItemId() const;
    int selectedHoldItemId() const;
};

#endif // PATRONWINDOW_H
