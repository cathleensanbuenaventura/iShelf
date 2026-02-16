#ifndef VIDEOGAME_H
#define VIDEOGAME_H

#include "catalogueitem.h"

class VideoGame : public CatalogueItem
{
public:
    VideoGame(int id,
              const QString &title,
              const QString &studio,
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

#endif // VIDEOGAME_H
