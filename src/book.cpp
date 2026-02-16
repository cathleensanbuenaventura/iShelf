#include "book.h"

// This concrete class delegates most data to the CatalogueItem base class.
FictionBook::FictionBook(int id,
                         const QString &title,
                         const QString &author,
                         int publicationYear,
                         const QString &isbn)
    : CatalogueItem(id, title, author, publicationYear, isbn, ItemType::FictionBook)
{
}

QString FictionBook::formatName() const
{
    return "Fiction Book";
}

// Concrete class representing a non-fiction book, which also stores a Dewey class.
NonFictionBook::NonFictionBook(int id,
                               const QString &title,
                               const QString &author,
                               int publicationYear,
                               const QString &isbn,
                               const QString &dewey)
    : CatalogueItem(id, title, author, publicationYear, isbn, ItemType::NonFictionBook),
      m_deweyClass(dewey)
{
}

QString NonFictionBook::formatName() const
{
    return "Non-Fiction Book";
}

QString NonFictionBook::deweyClass() const
{
    return m_deweyClass;
}
