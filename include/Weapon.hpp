#pragma once

#include <string>
#include <utility>
#include <unordered_map>
#include "Item.hpp"

namespace rp
{

/**
 * \brief Enumeration of the types of weapons
 */
enum class WeapType {
	Sword,	///< Swords
	Lance,	///< Lances
	Axe,		///< Axes
	Fire,		///< Fire magic
	Wind,		///< Wind magic
};

/**
 * \brief Class representing a weapon. Weapons are a subgroup of items.
 */
class Weapon : public Item
{
public:

	/**
	 * \brief Definition of structure containing a weapon's stats. This 
	 * structure is what Stats() returns, and it is what's stored for each 
	 * weapon in the weapon archive, which is used in the construction of new 
	 * weapons.
	 * 
	 * \see Stats
	 */
	using WeapStats = struct WeapStats
	{
		///< Name of the weapon (example: "Iron Lance")
		std::string name;
		///< Weapon's type (example: WeapType::Lance")
		WeapType type;
		///< Minimum level the owner has to be in order to be equipped with this
		// this weapon (example: 45)
		int level;
		///< Weapon's power
		int might;
		///< Weapon's accuracy
		int hit;
		///< Chance of critical hit with this weapon
		int crit;
		///< Range of attack, expressed in two endpoints. The first number is the
		// shortest distance possible while the second is the farthest. (example: 
		// {1, 2})
		std::pair<int, int> range;
		///< Number of times the weapon can be used before it breaks
		int uses;
	};

	/**
	 * \brief Construct a weapon
	 * 
	 * This constructor creates a weapon based on \a id. It retrieves stats from
	 * the weapon archive that corresonds to \a id. These stats are contained in 
	 * a WeapStats structure.
	 * 
	 * \see WeapStats
	 * 
	 * The constructor stores the info in its class. The reason for storing a 
	 * local copy of the stats instead of looking them up in the archive every 
	 * time is that these stats can change over time, from one another. The 
	 * number of remaining uses is the obvious case.
	 * 
	 * This constructor causes an assert error if the weapon's ID is not found 
	 * in the archive. This can happen if the programmer redefined the ItemID 
	 * enum class but didn't add in the ID and stats into the archive. This can, 
	 * of course, also happen if the argument is an item that doesn't belong to 
	 * the weapon subgroup, which, again, is defined in the weapon archive.
	 * 
	 * This constructor does not assign an owner to the weapon. The owner by 
	 * default is set to CharID::None. Call SetOwner() to assign the weapon to 
	 * an owner.
	 * 
	 * \param id		Item's ID
	 */
	Weapon(ItemID id);

	/**
	 * \brief Get the name of the weapon
	 * 
	 * \return Weapon's name
	 */
	std::string Name() const override;

	/**
	 * \brief Get the weapon's stats
	 * 
	 * \return Weapon's stats
	 */
	WeapStats Stats() const;

	/**
	 * \brief Checks if the weapon has run out of uses
	 * 
	 * \return True if yes; false otherwise.
	 */
	bool IsUsable() const override;

	/**
	 * \brief Lower weapon durability 
	 * 
	 * With a few exceptions, every weapon can be used only a certain number of 
	 * times before they break i.e. \property m_stats.uses becomes 0. This 
	 * method reduces the number of times the weapon can be used by one. If 
	 * \property Stats().uses returns 0 or \property IsBroken() returns true, 
	 * then this method does nothing. It is the client's responsibility to call 
	 * this function whenever the weapon is used in order to keep proper count.
	 */
	void Tear();

	/**
	 * \brief Raise weapon's durability
	 * 
	 * This method increases the remaining number of times the weapon can be used
	 * by a specified number, prolonging the life of the weapon. Note that it is
	 * possible for the weapon's durability to exceed its initial number through
	 * this method.
	 * 
	 * \param uses		Extra uses for weapon
	 */
	void Restore(const decltype(WeapStats::uses) uses);

private:
	/**
	 * \brief Weapon archive
	 * 
	 * To add a new weapon to the game, not only does the WeapID enum class need 
	 * to be redefined, but its ID and stats must be added here. Otherwise, if 
	 * you try to construct the new weapon, the constructor will fail because it 
	 * can't find is info in the archive. The archive is an unordered map, where
	 * 
	 * \li the key is the weapon's ID, and
	 * \li the mapped value is the weapon's stats
	 */
	static std::unordered_map<ItemID, WeapStats> m_db;
	
	// Weapon's stats are stored here
	WeapStats m_stats;
};

}