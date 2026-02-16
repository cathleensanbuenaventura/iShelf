#ifndef CATALOGUEITEM_H
#define CATALOGUEITEM_H

#include <QString>
#include <QDate>
#include <deque>

// High-level type of a catalogue item.
// Used by the system to distinguish which concrete subclass to create/use.
enum class ItemType {
    FictionBook,
    NonFictionBook,
    Magazine,
    Movie,
    VideoGame
};

enum class ItemStatus {
    Available,
    CheckedOut
};


// Abstract base class for all items stored in the library catalogue.
// Holds fields common to all item types and defines a small polymorphic interface
// that subclasses (books, magazines, movies, games) can override.
class CatalogueItem
{
public:
    CatalogueItem(int id,
                  const QString &title,
                  const QString &creator,
                  int publicationYear,
                  const QString &isbn,
                  ItemType type);
    virtual ~CatalogueItem() = default;

    int id() const;
    const QString &title() const;
    const QString &creator() const;
    int publicationYear() const;
    const QString &isbn() const;

    ItemType type() const;

    // Lending status and current borrower information.
    ItemStatus status() const;
    void setStatus(ItemStatus status);

    int currentBorrowerId() const;
    void setCurrentBorrowerId(int borrowerId);

    const QDate &dueDate() const;
    void setDueDate(const QDate &date);

    // FIFO queue of patron ids waiting for this item.
    std::deque<int> &holdQueue();
    const std::deque<int> &holdQueue() const;

    virtual QString formatName() const = 0;

    // Optional, type-specific data; subclasses override only what they need.
    virtual QString deweyClass() const { return QString(); }
    virtual QString issueNumber() const { return QString(); }
    virtual QDate publicationDate() const { return QDate(); }
    virtual QString genre() const { return QString(); }
    virtual QString rating() const { return QString(); }

protected:
    int m_id;
    QString m_title;
    QString m_creator;
    int m_publicationYear;
    QString m_isbn;

    ItemType m_type;
    ItemStatus m_status;
    int m_currentBorrowerId;
    QDate m_dueDate;
    std::deque<int> m_holdQueue;
};

#endif // CATALOGUEITEM_H
