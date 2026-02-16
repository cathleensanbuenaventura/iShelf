#ifndef CATALOGUEITEMFACTORY_H
#define CATALOGUEITEMFACTORY_H

#include <memory>
#include <QDate>
#include "book.h"
#include "magazine.h"
#include "movie.h"
#include "videogame.h"

class CatalogueItemFactory
{
public:
    virtual ~CatalogueItemFactory() = default;

    virtual std::unique_ptr<CatalogueItem> createFictionBook(
            int id,
            const QString &title,
            const QString &author,
            int publicationYear,
            const QString &isbn) = 0;

    virtual std::unique_ptr<CatalogueItem> createNonFictionBook(
            int id,
            const QString &title,
            const QString &author,
            int publicationYear,
            const QString &isbn,
            const QString &deweyClass) = 0;

    virtual std::unique_ptr<CatalogueItem> createMagazine(
            int id,
            const QString &title,
            const QString &publisher,
            int publicationYear,
            const QString &isbn,
            const QString &issueNumber,
            const QDate &pubDate) = 0;

    virtual std::unique_ptr<CatalogueItem> createMovie(
            int id,
            const QString &title,
            const QString &director,
            int publicationYear,
            const QString &genre,
            const QString &rating) = 0;

    virtual std::unique_ptr<CatalogueItem> createVideoGame(
            int id,
            const QString &title,
            const QString &studio,
            int publicationYear,
            const QString &genre,
            const QString &rating) = 0;
};

class DefaultCatalogueItemFactory : public CatalogueItemFactory
{
public:
    std::unique_ptr<CatalogueItem> createFictionBook(
            int id,
            const QString &title,
            const QString &author,
            int publicationYear,
            const QString &isbn) override;

    std::unique_ptr<CatalogueItem> createNonFictionBook(
            int id,
            const QString &title,
            const QString &author,
            int publicationYear,
            const QString &isbn,
            const QString &deweyClass) override;

    std::unique_ptr<CatalogueItem> createMagazine(
            int id,
            const QString &title,
            const QString &publisher,
            int publicationYear,
            const QString &isbn,
            const QString &issueNumber,
            const QDate &pubDate) override;

    std::unique_ptr<CatalogueItem> createMovie(
            int id,
            const QString &title,
            const QString &director,
            int publicationYear,
            const QString &genre,
            const QString &rating) override;

    std::unique_ptr<CatalogueItem> createVideoGame(
            int id,
            const QString &title,
            const QString &studio,
            int publicationYear,
            const QString &genre,
            const QString &rating) override;
};

#endif // CATALOGUEITEMFACTORY_H
