#ifndef USER_H
#define USER_H

#include <QString>
#include <vector>

// Role of a user in the system; determines which UI and permissions they get.
enum class UserRole {
    Patron,
    Librarian,
    Administrator
};

// Simple domain object representing a system user.
// For patrons, also tracks current loans and holds.
class User
{
public:
    User(int id, const QString &name, UserRole role);

    int id() const;
    const QString &name() const;
    UserRole role() const;

    // Patron data (used only when role == Patron)
    std::vector<int> &activeLoans();
    const std::vector<int> &activeLoans() const;

    // activeHolds contains item ids this patron has on hold.
    std::vector<int> &activeHolds();
    const std::vector<int> &activeHolds() const;

private:
    int m_id;
    QString m_name;
    UserRole m_role;

    std::vector<int> m_activeLoans;
    std::vector<int> m_activeHolds;
};

#endif // USER_H
