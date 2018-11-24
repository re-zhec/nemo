#include <algorithm>
#include <cassert>
#include "../include/logger.hpp"
#include "../include/Inventory.hpp"
#include "../include/Item.hpp"

namespace rp
{

Inventory::Inventory()
	: m_capacity(1000)
	, m_weight(0)
{
	// In the worst case scenario, every item in inventory is unique i.e. there // are no multiple copies of any item. So chronological list and storage 
	// should reserve space up to inventory capacity. This is to prevent forced // reallocations of memory for the chronological list, which is a vector, 
	// and rehashes for the storage, which is an unordered map, upon new
	// insertions. 
	m_order.reserve(m_capacity);
	m_storage.reserve(m_capacity);
}


bool Inventory::Add(std::shared_ptr<Item> item)
{
	if (m_weight == m_capacity) {
		// Storage is full. No more items can be added
		return false;
	}
	
	++m_weight;

	// Update chronological list
	auto id = item->ID();
	auto it = std::find(m_order.cbegin(), m_order.cend(), id);

	// The list is ordered from oldest IDs at the front of the vector to newest
	// at the back. If the item is already in the list, and the new one added is 
	// a duplicate, remove the item's ID from the vector and reinsert it at the 
	// back. The removal will cause a memory reallocation in most scenarios, but 
	// we're only dealing with 1000 ints => 4000 bytes at worst. 
	if (it != m_order.cend()) {
		m_order.erase(it);
	}

	m_order.push_back(id);

	// Add item to the storage
	m_storage[id].push_back(item);
	return true;
}


void Inventory::RemoveGroup(const ItemID id)
{
	// Delete copies of the item in storage
	auto s_iter = m_storage.find(id);
	assert(s_iter != m_storage.cend());
	m_weight -= s_iter->second.size();
	m_storage.erase(s_iter);

	// Remove item's ID from chronological list
	auto o_iter = std::find(m_order.cbegin(), m_order.cend(), id);
	assert(o_iter != m_order.cend());
	m_order.erase(o_iter);

	LOG_DEBUG("Deleted all of item group " << id);
}


void Inventory::RemoveOne(const ItemID id, const size_t which)
{
	// look up item group in storage
	auto s_iter = m_storage.find(id);
	assert(s_iter != m_storage.cend());

	// Make sure index is within container size
	auto& mults = s_iter->second;
	assert(which < mults.size());

	if (mults.size() == 1) {
		// This is the only copy left, so delete the entire item category
		RemoveGroup(id);
	}
	else {
		auto i_iter = mults.begin() + which;
		mults.erase(i_iter);
		--m_weight;

		LOG_DEBUG("deleted 1 of item group " << id);
	}
}


std::vector<std::shared_ptr<Item>>& Inventory::SelectGroup(const ItemID id)
{
	auto it = m_storage.find(id);
	assert(it != m_storage.cend());
	return it->second;
}


std::shared_ptr<Item>& Inventory::SelectOne(const ItemID id, const size_t which)
{
	auto mults = SelectGroup(id);
	assert(which < mults.size());
	return mults[which];
}


void Inventory::Overview() const &
{
	// In the order they are found or bought
	for (auto id : m_order) {
		// look up id in storage
		auto it = m_storage.find(id);
		assert(it != m_storage.cend());

		// Display name and count
		auto mults = it->second;
		std::cout << mults[0]->Name() << ": " << mults.size() << std::endl;
	}
}

}