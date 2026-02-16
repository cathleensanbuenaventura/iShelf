#ifndef BOOK_H
#define BOOK_H

#include "catalogueitem.h"

class FictionBook : public CatalogueItem
{
public:
    FictionBook(int id,
                const QString &title,
                const QString &author,
                int publicationYear,
                const QString &isbn);

    QString formatName() const override;
};

class NonFictionBook : public CatalogueItem
{
public:
    NonFictionBook(int id,
                   const QString &title,
                   const QString &author,
                   int publicationYear,
                   const QString &isbn,
                   const QString &dewey);

    QString formatName() const override;
    QString deweyClass() const override;

private:
    QString m_deweyClass;
};

#endif // BOOK_H
