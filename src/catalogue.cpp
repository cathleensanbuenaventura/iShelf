#include "catalogue.h"

Catalogue::Catalogue() = default;

// Add a new item to the catalogue.
void Catalogue::addItem(std::unique_ptr<CatalogueItem> item)
{
    m_items.push_back(std::move(item));
}

// Read-only access to all catalogue items.
// Used when callers only need to inspect items, not modify the container.
const std::vector<std::unique_ptr<CatalogueItem>> &Catalogue::items() const
{
    return m_items;
}

std::vector<std::unique_ptr<CatalogueItem>> &Catalogue::items()
{
    return m_items;
}

// Find a catalogue item by its unique id (non-const version).
// Returns a raw pointer to the item, or nullptr if not found.
CatalogueItem *Catalogue::findItemById(int id)
{
    for (auto &ptr : m_items) {
        if (ptr->id() == id) {
            return ptr.get();
        }
    }
    return nullptr;
}

// Const overload of findItemById, used when the Catalogue itself is const.
// Also returns nullptr when no matching item exists.
const CatalogueItem *Catalogue::findItemById(int id) const
{
    for (const auto &ptr : m_items) {
        if (ptr->id() == id) {
            return ptr.get();
        }
    }
    return nullptr;
}
