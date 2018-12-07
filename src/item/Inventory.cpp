#include <algorithm>
#include <boost/assert.hpp>
#include <boost/range/adaptor/reversed.hpp>

#include "Inventory.hpp"

namespace nemo
{

////////////////////////////////////////////////////////////////////////////////
//																										//
////////////////////////////////////////////////////////////////////////////////

Inventory::Inventory(const size_t capacity)
	: m_capacity(capacity)
	, m_weight(0)
{
	BOOST_ASSERT(m_capacity > 0);

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
	const auto [it, UNUSED_] = find(id);

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

[[nodiscard]]
std::optional< std::pair<std::shared_ptr<Item>, size_t> >
Inventory::remove(const int id, const size_t which)
{
	// Find where this item is in the storage and chronological list. The latter
	// is needed if this item is the last of its kind left in the inventory.
	auto [it_order, it_storage] = find(id);
	BOOST_ASSERT(it_order != m_order.cend());
	BOOST_ASSERT(it_storage != m_storage.cend());
	
	auto& [UNUSED_, copies] = *it_storage;
	BOOST_ASSERT(which < copies.size());

	// Remove the selected copy.
	const auto item = copies[which];
	copies.erase(copies.cbegin() + which);
	const auto n_remain = copies.size();

	--m_weight;

	if (n_remain == 0) {
		// That was the last one, so remove the metadata attached to the item.
		m_storage.erase(it_storage);
		m_order.erase(it_order);
	}

	return { {item, n_remain} };
}

////////////////////////////////////////////////////////////////////////////////
//																										//
////////////////////////////////////////////////////////////////////////////////

std::vector<std::tuple<int, std::string, size_t>>
Inventory::peek() const
{
	std::vector<std::tuple<int, std::string, size_t>> inside;
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

auto
Inventory::find(const int id) 
-> decltype(std::make_pair(m_order.begin(), m_storage.begin()))
{
	auto it_order = std::find(m_order.begin(), m_order.end(), id);
	auto it_storage = m_storage.find(id);
	return std::make_pair(it_order, it_storage);
}

////////////////////////////////////////////////////////////////////////////////
//																										//
////////////////////////////////////////////////////////////////////////////////

}