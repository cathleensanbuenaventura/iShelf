#ifndef LIBRARIANWINDOW_H
#define LIBRARIANWINDOW_H

#include <QMainWindow>

class HinLIBSSystem;

// Main UI window for librarian users.
// Provides access to catalogue management and returning items on behalf of patrons.
class LibrarianWindow : public QMainWindow
{
    Q_OBJECT
public:
    explicit LibrarianWindow(HinLIBSSystem *system, int userId, QWidget *parent = nullptr);

signals:
    void logoutRequested();

private slots:
    // Slots connected to the three main librarian actions.
    void onAddItemClicked();
    void onRemoveItemClicked();
    void onReturnForPatronClicked();

private:
    HinLIBSSystem *m_system; // Pointer to the core system logic used by this window.

};

#endif // LIBRARIANWINDOW_H
