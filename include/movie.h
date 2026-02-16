#ifndef MOVIE_H
#define MOVIE_H

#include "catalogueitem.h"

class Movie : public CatalogueItem
{
public:
    Movie(int id,
          const QString &title,
          const QString &director,
          int publicationYear,
          const QString &genre,
          const QString &rating);

    QString formatName() const override;
    QString genre() const override;
    QString rating() const override;

private:
    QString m_genre;
    QString m_rating;
};

#endif // MOVIE_H
