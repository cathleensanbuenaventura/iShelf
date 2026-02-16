#include "videogame.h"

// Concrete catalogue item representing a video game.
// Stores the studio plus game-specific metadata (genre and rating).
VideoGame::VideoGame(int id,
                     const QString &title,
                     const QString &studio,
                     int publicationYear,
                     const QString &genre,
                     const QString &rating)
    : CatalogueItem(id, title, studio, publicationYear, QString(), ItemType::VideoGame),
      m_genre(genre),
      m_rating(rating)
{
}

QString VideoGame::formatName() const
{
    return "Video Game";
}

// Return the game's genre (e.g., RPG, Adventure).
QString VideoGame::genre() const
{
    return m_genre;
}

// Return the game's rating (e.g., E, T, M).
QString VideoGame::rating() const
{
    return m_rating;
}
