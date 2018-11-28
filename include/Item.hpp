#pragma once
#include <string>

namespace rp
{

/**
 * \brief Abstract class representing an item, such as weapons, armors, healing 
 * items, etc.. . It is inherited by the \var Weapon class.
 */
class Item
{
public:
	/**
	 * \brief Construct an item
	 * 
	 * Since the construction of the item depends on what type of item it is, 
	 * the constructor is mostly empty aside from copying the ID.
	 * 
	 * \param id		Item's ID
	 */
	Item(int id);

	/**
	 * \brief Destructs an item
	 * 
	 * The items are shared across inventories and party character's equipments 
	 * through shared pointers, so a destructor that logs when the item is 
	 * discarded would be beneficial.
	 */
	~Item();

	/**
	 * \brief Get the item's ID
	 * 
	 * \return Item's ID
	 */
	int ID() const noexcept;

	/**
	 * \brief Get the name of the item
	 * 
	 * When inherited by the \var Weapon class, this method returns the name of 
	 * the weapon.
	 * 
	 * \return Weapon's name
	 */
	std::string Name() const;
	
private:
	int m_id;			// Item's ID
	std::string m_name;
};

}