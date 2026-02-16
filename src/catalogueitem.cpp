#include "catalogueitem.h"

// Base class representing a generic item in the library catalogue.
// Stores fields common to all item types.
CatalogueItem::CatalogueItem(int id,
                             const QString &title,
                             const QString &creator,
                             int publicationYear,
                             const QString &isbn,
                             ItemType type)
    : m_id(id),
      m_title(title),
      m_creator(creator),
      m_publicationYear(publicationYear),
      m_isbn(isbn),
      m_type(type),
      m_status(ItemStatus::Available),
      m_currentBorrowerId(-1)
{
}

// Basic getters for immutable item metadata.
int CatalogueItem::id() const { return m_id; }
const QString &CatalogueItem::title() const { return m_title; }
const QString &CatalogueItem::creator() const { return m_creator; }
int CatalogueItem::publicationYear() const { return m_publicationYear; }
const QString &CatalogueItem::isbn() const { return m_isbn; }

ItemType CatalogueItem::type() const { return m_type; }

ItemStatus CatalogueItem::status() const { return m_status; }
void CatalogueItem::setStatus(ItemStatus status) { m_status = status; }

// ID of the patron currently borrowing this item (-1 means no active borrower).
int CatalogueItem::currentBorrowerId() const { return m_currentBorrowerId; }
void CatalogueItem::setCurrentBorrowerId(int borrowerId) { m_currentBorrowerId = borrowerId; }

// Due date of the current loan. Empty QDate if not checked out.
const QDate &CatalogueItem::dueDate() const { return m_dueDate; }
void CatalogueItem::setDueDate(const QDate &date) { m_dueDate = date; }

// FIFO hold queue storing patron IDs waiting for this item.
std::deque<int> &CatalogueItem::holdQueue() { return m_holdQueue; }
const std::deque<int> &CatalogueItem::holdQueue() const { return m_holdQueue; }
