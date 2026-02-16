#include "loginwindow.h"
#include "hinlibssystem.h"
#include "patronwindow.h"
#include "librarianwindow.h"
#include "adminwindow.h"

#include <QLineEdit>
#include <QPushButton>
#include <QVBoxLayout>
#include <QLabel>
#include <QMessageBox>
#include <QWidget>

// Simple login window that asks for a username and opens the
// appropriate UI (Patron, Librarian, or Admin) based on user role.
LoginWindow::LoginWindow(HinLIBSSystem *system, QWidget *parent)
    : QMainWindow(parent),
      m_system(system)
{
    QWidget *central = new QWidget(this);
    QVBoxLayout *layout = new QVBoxLayout(central);

    QLabel *label = new QLabel("Enter username:", central);
    m_usernameEdit = new QLineEdit(central);
    m_loginButton = new QPushButton("Enter", central);

    layout->addWidget(label);
    layout->addWidget(m_usernameEdit);
    layout->addWidget(m_loginButton);

    setCentralWidget(central);
    setWindowTitle("HinLIBS - Login");

    // When the user clicks Enter, handle the login attempt.
    connect(m_loginButton, &QPushButton::clicked,
            this, &LoginWindow::onLoginClicked);
}

void LoginWindow::onLoginClicked()
{
    QString name = m_usernameEdit->text().trimmed();
    if (name.isEmpty()) {
        QMessageBox::warning(this, "Login", "Please enter a username.");
        return;
    }

    // Ask the system to find the user and determine their role.
    auto result = m_system->findUserByName(name);
    if (!result.found) {
        QMessageBox::warning(this, "Login", "User not found.");
        return;
    }

    // Open the appropriate window based on the user role.
    switch (result.role) {
    case UserRole::Patron:
        openPatronWindow(result.userId);
        break;
    case UserRole::Librarian:
        openLibrarianWindow(result.userId);
        break;
    case UserRole::Administrator:
        openAdminWindow(result.userId);
        break;
    }
}

// Create and show the Patron window for the given patron id.
void LoginWindow::openPatronWindow(int patronId)
{
    auto *w = new PatronWindow(m_system, patronId, this);
    w->setAttribute(Qt::WA_DeleteOnClose);
    w->show();
    this->hide();
    connect(w, &PatronWindow::logoutRequested, [this]() {
        m_usernameEdit->clear();
        this->show();
    });
}

// Create and show the Librarian window for the given user id.
void LoginWindow::openLibrarianWindow(int userId)
{
    auto *w = new LibrarianWindow(m_system, userId, this);
    w->setAttribute(Qt::WA_DeleteOnClose);
    w->show();
    this->hide();
    connect(w, &LibrarianWindow::logoutRequested, [this]() {
        m_usernameEdit->clear();
        this->show();
    });
}

// Create and show the Admin window for the given user id.
void LoginWindow::openAdminWindow(int userId)
{
    auto *w = new AdminWindow(m_system, userId, this);
    w->setAttribute(Qt::WA_DeleteOnClose);
    w->show();
    this->hide();
    connect(w, &AdminWindow::logoutRequested, [this]() {
        m_usernameEdit->clear();
        this->show();
    });
}
