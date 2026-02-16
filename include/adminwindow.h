#ifndef ADMINWINDOW_H
#define ADMINWINDOW_H

#include <QMainWindow>

class HinLIBSSystem;

class AdminWindow : public QMainWindow
{
    Q_OBJECT
public:
    explicit AdminWindow(HinLIBSSystem *system, int userId, QWidget *parent = nullptr);

signals:
    void logoutRequested();
};

#endif // ADMINWINDOW_H
