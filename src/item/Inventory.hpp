#pragma once

#include <vector>
#include <unordered_map>
#include <memory>
#include <optional>

#include "Item.hpp"

namespace sb
{

/**
 * \brief Player's inventory of items.
 * 
 * Any item the player finds or buys is added here. Any item that is the player
 * discards or sells is to be removed from here. The player should be able to 
 * see both unequipped and equipped items (in the case of weapons and armors) 
 * here, so, all items in the inventory are stored as shared pointers. Any 
 * changes to the item's info, such as any upgrades, will be reflected in the 
 * inventory.
 */
class Inventory
{
public:
	/**
	 * \brief Construct an empty inventory.
	 * 
	 * This constructor generates an assertion error if \a capacity is 0.
	 * 
	 * \param capacity		Maximum number of items the inventory can hold.
	 */
	Inventory(const size_t capacity);

	/**
	 * \brief Add an item to the inventory.
	 * 
	 * Any further calls to this method when the inventory is full will do 
	 * nothing. The item will not be added. Multiple copies of the same count 
	 * toward the capacity of the inventory. 
	 * 
	 * \param item		Shared pointer to the item to be added.
	 * 	
	 * \return True if the item has been successfully added; false otherwise.
	 */
	bool add(std::shared_ptr<Item> item);

	/**
	 * \brief Remove an item from the inventory.
	 * 
	 * Since it is possible to have duplicates of the same item e.g. multiple 
	 * Potions, \a which determines which one to remove. If there is only one 
	 * copy, this argument should be 0. This method generates an assertion error 
	 * if the item cannot be found in the inventory via \a id. It also generates 
	 * one if \a which is out of range.
	 * 
	 * \param id		ID of the item group of which one is to be removed.
	 * \param which	0-based index of the item to remove.
	 * 
	 * \return The removed item and how many of them are left in the inventory 
	 * after the removal, or nullopt if the item cannot be removed for some 
	 * reason.
	 */
	[[nodiscard]]
	std::optional< std::pair<std::shared_ptr<Item>, size_t> >
	remove(const int id, const size_t which);

	/**
	 * \brief Remove all of this item from the inventory.
	 * 
	 * Instead of removing just one copy, this removes all of the same item. It 
	 * can be called even if there's only one copy left. It will behave like the 
	 * above overloaded method. It generates an assertion error if the item 
	 * cannot be found in the inventory via \a id.
	 * 
	 * \param id		ID of the item to remove all of.
	 */
	void remove(const int id);

	/**
	 * \brief Get the IDs, names, and quantity of all items currentlyin the 
	 * inventory.
	 * 
	 * \return A vector, ordered from most recently found item to least recently, 
	 * containing for each item:
	 * 	1. its ID
	 * 	2. its name
	 * 	3. its quantity
	 */
	std::vector<std::tuple<int, std::string, size_t>> 
	peek() const;

private:
	///< Capacity.
	const size_t m_capacity;

	///< Current number of items in the inventory.
	size_t m_weight;

	///< Item storage is implemented as an unordered map, where each key/value 
	// pair corresponds to an item.
	//		Key: Item's ID
	//		Value: Objects for that item
	std::unordered_map< 
		int, 
		std::vector<std::shared_ptr<Item>> 
	> m_storage;

	///< Players normally expect an item they just found to be at the top of the 
	// inventory when they look for it. However, the storage is unordered for 
	// (potential) performance. Thus, a vector of IDs representing items 
	// currently in the inventory is maintained, ordered from least recently 
	// found to most recently. This order is opposite of that returned by  
	// \property peek. 
	std::vector<int> m_order;

	/**
	 * \brief Search for this item in the inventory.
	 * 
	 * This method finds the item that matches \a id. It SHOULDN'T find more 
	 * than one since \property add disallows multiple menu options having 
	 * the same ID, but if for some reason there is, it grabs the first 
	 * occurrence.
	 * 
	 * \param id		ID of the item to find.
	 * 
	 * \return A pair containing, in this order:
	 * 	1. A non-const iterator to the ID of the found item from the 
	 * 		chronological list. If there is no match, \var m_order.end() is 
	 * 		returned.
	 * 	2. A non-const iterator to all copies of the found item from storage.
	 * 		If there is no match, \var m_storage.end() is returned.
	 */
	auto find(const int id) 
		-> decltype(std::make_pair(m_order.begin(), m_storage.begin()));
};

}