#include <algorithm>
#include <cassert>
#include <boost/range/adaptor/reversed.hpp>
#include "../include/Inventory.hpp"

namespace rp
{

////////////////////////////////////////////////////////////////////////////////
//																										//
////////////////////////////////////////////////////////////////////////////////

Inventory::Inventory(const size_t capacity)
	: m_capacity(capacity)
	, m_weight(0)
{
	assert(m_capacity > 0);

	// In the worst case scenario, every item in inventory is unique i.e. there 
	// are no extra copies of any item. The chronological list and storage should 
	// reserve space up to inventory capacity.
	m_order.reserve(m_capacity);
	m_storage.reserve(m_capacity);
}

////////////////////////////////////////////////////////////////////////////////
//																										//
////////////////////////////////////////////////////////////////////////////////

bool 
Inventory::add(std::shared_ptr<Item> item)
{
	if (m_weight == m_capacity) {
		// Storage is full. No more items can be added.
		return false;
	}
	
	++m_weight;

	// Update the chronological list of items obtained.
	const auto id = item->ID();
	const auto it = std::find(m_order.cbegin(), m_order.cend(), id);

	// The container of IDs is ordered from the oldest item at the front to the 
	// newest at the back. If the added item already exists in the inventory, 
	// then move its ID to the back of the list.
	if (it != m_order.cend()) {
		m_order.erase(it);
	}

	m_order.push_back(id);

	// Add the item to the storage.
	m_storage[id].push_back(item);
	return true;
}

////////////////////////////////////////////////////////////////////////////////
//																										//
////////////////////////////////////////////////////////////////////////////////

void 
Inventory::remove(const ItemID id, const size_t which)
{
	// Find where this item is in the storage and chronological list. The latter
	// is needed if this item is the last of its kind left in the inventory.
	const auto storage_it = m_storage.find(id);
	const auto order_it = std::find(m_order.cbegin(), m_order.cend(), id);
	assert(order_it != m_order.cend());
	assert(storage_it != m_storage.cend());

	// Delete the selected copy.
	auto& [UNUSED_, copies] = *storage_it;
	assert(which < copies.size());
	copies.erase(copies.cbegin() + which);
	--m_weight;

	if (copies.empty()) {
		// That was the last one, so remove the metadata attached to the item.
		m_storage.erase(storage_it);
		m_order.erase(order_it);
	}
}

////////////////////////////////////////////////////////////////////////////////
//																										//
////////////////////////////////////////////////////////////////////////////////

std::vector<std::tuple<ItemID, std::string, size_t>>
Inventory::peek() const
{
	std::vector<std::tuple<ItemID, std::string, size_t>> inside;
	inside.reserve(m_order.size());

	// Start from the last element instead of the first, since the chronological 
	// list of IDs is ordered from least recently found to most recently but the 
	// opposite order is expected.
	for (auto id : boost::adaptors::reverse(m_order)) {
		const auto& copies = m_storage.at(id);
		inside.push_back({id, copies[0]->Name(), copies.size()});
	}

	return inside;
}

////////////////////////////////////////////////////////////////////////////////
//																										//
////////////////////////////////////////////////////////////////////////////////

}