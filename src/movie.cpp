#include "movie.h"

// Concrete catalogue item representing a movie.
// Stores director plus movie-specific metadata such as genre and rating.
Movie::Movie(int id,
             const QString &title,
             const QString &director,
             int publicationYear,
             const QString &genre,
             const QString &rating)
    : CatalogueItem(id, title, director, publicationYear, QString(), ItemType::Movie),
      m_genre(genre),
      m_rating(rating)
{
}

QString Movie::formatName() const
{
    return "Movie";
}

// Return the movie's genre (e.g., Drama, Comedy).
QString Movie::genre() const
{
    return m_genre;
}

// Return the movie's rating (e.g., PG, PG-13, R).
QString Movie::rating() const
{
    return m_rating;
}
