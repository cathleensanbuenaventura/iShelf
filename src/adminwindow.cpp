#include "adminwindow.h"
#include "hinlibssystem.h"
#include <QVBoxLayout>
#include <QLabel>
#include <QPushButton>
#include <QWidget>

AdminWindow::AdminWindow(HinLIBSSystem *, int, QWidget *parent)
    : QMainWindow(parent)
{
    // Create the central widget and a vertical layout to hold all admin controls.
    QWidget *central = new QWidget(this);
    QVBoxLayout *layout = new QVBoxLayout(central);
    layout->addWidget(new QLabel("Admin features not implemented in D1.", central));

    // Logout button to return to the login window.
    auto *btn = new QPushButton("Logout", central);
    layout->addWidget(btn);
    setCentralWidget(central);
    setWindowTitle("HinLIBS - Admin");

    // When Logout is clicked, emit logoutRequested() so LoginWindow can react,
    // then close this window.
    connect(btn, &QPushButton::clicked, [this]() {
        emit logoutRequested();
        close();
    });
}
