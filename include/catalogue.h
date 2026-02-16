#ifndef CATALOGUE_H
#define CATALOGUE_H

#include <vector>
#include <memory>
#include "catalogueitem.h"

// Container class that owns all CatalogueItem objects in the system.
// Provides basic operations to add items and look them up by id.v
class Catalogue
{
public:
    Catalogue();

    void addItem(std::unique_ptr<CatalogueItem> item);

    const std::vector<std::unique_ptr<CatalogueItem>> &items() const;
    std::vector<std::unique_ptr<CatalogueItem>> &items();

    // Find an item by its unique id; returns nullptr if not found.
    CatalogueItem *findItemById(int id);
    const CatalogueItem *findItemById(int id) const;

private:
    // Owned collection of all catalogue items.
    std::vector<std::unique_ptr<CatalogueItem>> m_items;
};

#endif // CATALOGUE_H
