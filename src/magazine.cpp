#include "magazine.h"

// Concrete catalogue item representing a magazine issue.
// Stores publisher information plus issue number and publication date.
Magazine::Magazine(int id,
                   const QString &title,
                   const QString &publisher,
                   int publicationYear,
                   const QString &isbn,
                   const QString &issueNumber,
                   const QDate &pubDate)
    : CatalogueItem(id, title, publisher, publicationYear, isbn, ItemType::Magazine),
      m_issue(issueNumber),
      m_pubDate(pubDate)
{
}

QString Magazine::formatName() const
{
    return "Magazine";
}

// Return the magazine's issue identifier (e.g., "Issue 5").
QString Magazine::issueNumber() const
{
    return m_issue;
}

// Return the publication date of this issue.
QDate Magazine::publicationDate() const
{
    return m_pubDate;
}
