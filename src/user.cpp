#include "user.h"

// Simple domain object representing a system user (patron, librarian, or admin).
User::User(int id, const QString &name, UserRole role)
    : m_id(id), m_name(name), m_role(role)
{
}

// Basic getters for immutable identity and role.
int User::id() const { return m_id; }
const QString &User::name() const { return m_name; }
UserRole User::role() const { return m_role; }

// List of item ids currently on loan to this user (patrons only).
std::vector<int> &User::activeLoans() { return m_activeLoans; }
const std::vector<int> &User::activeLoans() const { return m_activeLoans; }

// List of item ids currently on loan to this user (patrons only).
std::vector<int> &User::activeHolds() { return m_activeHolds; }
const std::vector<int> &User::activeHolds() const { return m_activeHolds; }
