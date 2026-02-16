#include "catalogueitemfactory.h"


// Concrete factory responsible for constructing specific CatalogueItem subclasses.
// Each method creates the appropriate type and returns it as a unique_ptr<CatalogueItem>.

std::unique_ptr<CatalogueItem> DefaultCatalogueItemFactory::createFictionBook(
        int id,
        const QString &title,
        const QString &author,
        int publicationYear,
        const QString &isbn)
{
    return std::make_unique<FictionBook>(id, title, author, publicationYear, isbn);
}

std::unique_ptr<CatalogueItem> DefaultCatalogueItemFactory::createNonFictionBook(
        int id,
        const QString &title,
        const QString &author,
        int publicationYear,
        const QString &isbn,
        const QString &deweyClass)
{
    return std::make_unique<NonFictionBook>(id, title, author, publicationYear, isbn, deweyClass);
}

std::unique_ptr<CatalogueItem> DefaultCatalogueItemFactory::createMagazine(
        int id,
        const QString &title,
        const QString &publisher,
        int publicationYear,
        const QString &isbn,
        const QString &issueNumber,
        const QDate &pubDate)
{
    return std::make_unique<Magazine>(id, title, publisher, publicationYear, isbn, issueNumber, pubDate);
}

std::unique_ptr<CatalogueItem> DefaultCatalogueItemFactory::createMovie(
        int id,
        const QString &title,
        const QString &director,
        int publicationYear,
        const QString &genre,
        const QString &rating)
{
    return std::make_unique<Movie>(id, title, director, publicationYear, genre, rating);
}

std::unique_ptr<CatalogueItem> DefaultCatalogueItemFactory::createVideoGame(
        int id,
        const QString &title,
        const QString &studio,
        int publicationYear,
        const QString &genre,
        const QString &rating)
{
    return std::make_unique<VideoGame>(id, title, studio, publicationYear, genre, rating);
}
