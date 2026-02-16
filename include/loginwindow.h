#ifndef LOGINWINDOW_H
#define LOGINWINDOW_H

#include <QMainWindow>
#include "user.h"

class QLineEdit;
class QPushButton;
class HinLIBSSystem;

// Top-level login window for HinLIBS.
// Prompts for a username and opens the appropriate UI based on user role.
class LoginWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit LoginWindow(HinLIBSSystem *system, QWidget *parent = nullptr);

private slots:
    void onLoginClicked();

private:
    HinLIBSSystem *m_system; // Pointer to the core system model/controller.
    QLineEdit *m_usernameEdit; // Text box where the user types the username.
    QPushButton *m_loginButton; // Button to submit the login.

    // Helper functions to open the correct window for each user role.
    void openPatronWindow(int patronId);
    void openLibrarianWindow(int userId);
    void openAdminWindow(int userId);
};

#endif // LOGINWINDOW_H
