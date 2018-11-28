#pragma once
#include <string>
#include <ostream>

namespace rp
{
/**
 * \brief Enumeration of item IDs.
 */
enum class ItemID {
	BrzSwd,	///< Bronze Sword.
	IrnSwd,	///< Iron Sword.
	SteSwd,	///< Steel Sword.
	SlvSwd,	///< Silver Sword.

	BrzLan,	///< Bronze Lance.
	IrnLan,	///< Iron Lance.
	SteLan,	///< Steel Lance.
	SlvLan,	///< Silver Lance.

	BrzAxe,	///< Bronze Axe.
	IrnAxe,	///< Iron Axe.
	SteAxe,	///< Steel Axe.
	SlvAxe,	///< Silver Axe.

	Fire,		///< Fire.
	Elfire,	///< Elfire.
	Arcfire,	///< Arcfire.
	Bolgano,	///< Bolganone.

	Wind		///< Wind.
};

enum class CharID; // Forward declaration

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
	Item(ItemID id);

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
	ItemID ID() const;

	/**
	 * \brief Get the name of the item
	 * 
	 * When inherited by the \var Weapon class, this method returns the name of 
	 * the weapon.
	 * 
	 * \return Weapon's name
	 */
	virtual std::string Name() const = 0;

	/**
	 * \brief Check if the item is usuable.
	 * 
	 * When inherited by the \var Weapon class, this method indicates whether 
	 * the weapon isn't broken i.e. still has uses remaining.
	 * 
	 * \return True if the item is; false otherwise.
	 */
	virtual bool IsUsable() const = 0;

	/**
	 * \brief Get the ID of the item's owner
	 * 
	 * \return Owner's ID
	 */
	CharID GetOwner() const;

	/**
	 * \brief Assign an owner to the item via the party character's ID
	 * 
	 * If this item already belongs to an owner i.e. the owner ID isn't 
	 * CharID::None, then assigning a new owner to it will fail. In order to 
	 * change ownership, \property SetOwner must be called twice, once with \var 
	 * rp::CharID::None and again with the new owner's ID.
	 * 
	 * \return True if the weapon's owner is successfully set to the battle 
	 * unit's ID; false otherwise.
	 * 
	 * \see GetOwner
	 */
	bool SetOwner(CharID owner) &;
	
private:
	ItemID m_id;			// Item's ID
	CharID m_owner;		// Owner's ID
};

}