#pragma once
#include <vector>
#include <unordered_map>
#include <memory>
#include "Item.hpp"

namespace rp
{

/**
 * \brief Class for managing the player's inventory of items. 
 * 
 * Any item the player finds or buys is added here. Any item that is the player
 * discards or sells to be removed from here. The player should be able to see 
 * both unequipped and equipped items (in the case of weapons) here, so by 
 * design, all items in the inventory are stored as shared pointers. Thus, any 
 * changes to the item's info, such as its durability, will be reflected in the 
 * inventory. Up to 1000 items can be stored.
 */
class Inventory
{
public:
	/**
	 * \brief Construct inventory of items
	 * 
	 * This contructor creates an empty inventory.
	 */
	Inventory();

	/**
	 * \brief Add an item to the inventory
	 * 
	 * The capacity of the inventory is 1000 items, including multiple copies of
	 * the same item. Any further attempts to add an item after the capacity
	 * capacity is reached will fail, with the item not being added.
	 * 
	 * \param item		Shared pointer to item object
	 * 	
	 * \return True if item is added; false otherwise
	 */
	bool Add(std::shared_ptr<Item> item);

	/**
	 * \brief Remove all of the same item from the inventory
	 * 
	 * This method will generate an assert error if the argument is an item that 
	 * isn't in the inventory.
	 * 
	 * \param id		Item's ID
	 */
	void RemoveGroup(const ItemID id);

	/**
	 * \brief Remove a selected copy of the item from the inventory
	 * 
	 * This method generates an assert error if the argument is an item that 
	 * isn't in the inventory. It also generates an assert error if the selected 
	 * index is beyond the size of the container in which all duplicates of the
	 * same items are stored. In other words, if the inventory has only 3 
	 * Elfires, RemoveOne(rp::WeapID::Elfire, 5) will fail because index 5 is 
	 * out of bound.
	 * 
	 * \param id		Item's ID
	 * \param which	Which copy of the item
	 */
	void RemoveOne(const ItemID id, const size_t which);

	/**
	 * \brief Get all duplicates of this item from the inventory
	 * 
	 * This method generates an assert error for the same conditions as 
	 * RemoveGroup().
	 * 
	 * \param id		Item's ID
	 * 
	 * \return Vector containing all duplicates of the item
	 * 
	 * \see RemoveGroup
	 */
	std::vector<std::shared_ptr<Item>>& SelectGroup(const ItemID id);
	
	/**
	 * \brief Get selected copy of the item from the inventory.
	 * 
	 * This method generates an assert error for the same conditions as 
	 * RemoveOne()
	 * 
	 * \param id		Item's ID
	 * \param which	Which copy of the item
	 * 
	 * \return Selected copy of the item from the inventory
	 * 
	 * \see RemoveOne
	 */
	std::shared_ptr<Item>& SelectOne(const ItemID id, const size_t which);

	/**
	 * \brief Display inventory from the top level 
	 * 
	 * The top level is a list of names of the items and how many of each the 
	 * player has in the inventory.
	 */
	void Overview() const &;

private:
	// Capacity
	const size_t m_capacity;

	// Current number of items in inventory
	size_t m_weight;

	// Enforce chronological order of items with vector
	std::vector<ItemID> m_order;

	// Storage is implemented as an unordered map.
	std::unordered_map< ItemID, std::vector<std::shared_ptr<Item>> > m_storage;
};

}