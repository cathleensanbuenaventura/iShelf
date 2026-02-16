#ifndef MAGAZINE_H
#define MAGAZINE_H

#include "catalogueitem.h"

class Magazine : public CatalogueItem
{
public:
    Magazine(int id,
             const QString &title,
             const QString &publisher,
             int publicationYear,
             const QString &isbn,
             const QString &issueNumber,
             const QDate &pubDate);

    QString formatName() const override;
    QString issueNumber() const override;
    QDate publicationDate() const override;

private:
    QString m_issue;
    QDate m_pubDate;
};

#endif // MAGAZINE_H
