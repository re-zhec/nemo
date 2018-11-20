#pragma once
#include <string>
#include <vector>
#include <unordered_map>
#include <memory>

namespace fe
{

/**
 * \brief Enumeration of party characters' IDs
 */
enum class CharID {
	None,
	Chrom,
	Robin	
};

// Forward declarations
class Weapon;
class Armor;
enum class WeapType; 

class PartyChar
{
public:
	/**
	 * \brief Definition of structure containing stats of a character
	 * 
	 * \see Stats
	 */
	using CharStats = struct CharStats
	{
		///< Character's name
		std::string name;
		///< Health
		int hp;
		///< Amount of magic to use
		int mp;
		///< Power for physical attacks. The more power, the more damage the 
		// character can inflict to an opponent with a physical attack.
		int str;
		///< Defense against physical attacks. The more defense, the less damage
		// the character can take from a physical attack
		int con;
		///< Power for magical attacks. Similar logic as \var str, except with 
		// magical attacks
		int mag;
		///< Defense against magical attacks. Similar logic as \var con, except 
		// with magical attacks
		int res;
		///< Speed
		int spd;
		///< Number of squares the character can move during their turn in the 
		// battleground map
		int mov;
	};

	/**
	 * \brief Constructs a party character 
	 * 
	 * This constructor creates a party character based on \a id given. It 
	 * retrieves stats from the character archive that corresonds to \a id. 
	 * These stats are contained in a CharStats structure.
	 * 
	 * The constructor stores the info it retrieved in its class. The reason for 
	 * storing a local copy of the stats instead of looking them up in the 
	 * archive every time is that these stats can change over time, from one 
	 * another. Party characters upgrade as they level up.
	 * 
	 * This constructor generates an assert error if the character's ID is not 
	 * found in the archive. This can happen if the programmer redefined the 
	 * CharID enum class but didn't add in the ID and stats into the archive.
	 * 
	 * \param id		ID of the party character
	 */
	PartyChar(CharID id);

	/**
	 * \brief Destructs a party character
	 * 
	 * This destructors logs when a character is destroyed. This usually only 
	 * matters with enemy character units.
	 */
	~PartyChar();
	
	/**
	 * \brief Get the character's ID
	 * 
	 * \return Character's ID
	 */
	CharID ID() const;

	/**
	 * \brief Get the character's stats
	 * 
	 * \return Character's stats
	 */
	CharStats Stats() const;

	/**
	 * \brief Equip a weapon to the character. PartyChar::Equip
	 * 
	 * This method doesn't equip the weapon if the character cannot wield it, 
	 * either because their weapon type of choice don't match or because their 
	 * level of proficiency isn't enough for this weapon.
	 * 
	 * \param w		Object of the weapon to be equipped to the character
	 * 
	 * \return True if the weapon is equipped. False otherwise
	 */
	bool Equip(std::shared_ptr<Weapon> w) &;

	/**
	 * \brief Remove the weapon from the character's equipment
	 * 
	 * This method does nothing if the character is already weaponless.
	 */
	void Unequip() &;

	/**
	 * \brief Reduce the character's health
	 * 
	 * If the character already has 0 HP, then this method does nothing. The 
	 * character cannot have negative HP. This method generates an assert error
	 * if a negative value is passed for the argument.
	 * 
	 * \param dmg	Number of health points to take away
	 */
	void Hurt(int dmg) &;

	/**
	 * \brief Increase the character's health 
	 * 
	 * The character cannot have more than their max health \var m_cur_hp, so 
	 * this method caps at that point if the max is exceeded.  This method does
	 * nothing if the character is already dead i.e. has 0 HP. It also generates
	 * an assert error if the amount of health points to add back is negative.
	 * 
	 * \param amt	Number of health points to add
	 */
	void Heal(int amt) &;

private:
	/**
	 * \brief Party character archive
	 * 
	 * To add a new party character to the game, not only does the CharID enum 
	 * class need to be redefined, but its ID and stats must be added here. 
	 * Otherwise, if you try to construct the new character, the constructor 
	 * will fail because it can't find is info in the archive. The archive is an 
	 * unordered map, where
	 * 
	 * \li the key is the character's ID, and
	 * \li the mapped value is the character's base stats
	 */
	static std::unordered_map<CharID, CharStats> m_db;

	// Character's ID
	CharID m_id;

	// Current level
	int m_level;

	// Natural stats, unmodified by equipment, buffs, debuffs, etc.
	CharStats m_stats;

	// Equipped weapon
	std::shared_ptr<Weapon> m_weap;

	// Weapon type of choice
	WeapType m_weaptype;

	// Equipped armor
	std::shared_ptr<Armor> m_armor;

	// Current health, not to be confused with \var m_stats.hp,  which is the 
	// character's maximum health points
	decltype(m_stats.hp) m_cur_hp;

	// Current fuel for magic, not to be confused with \var m_stats.mp, which is 
	// the character's maximum magic fuel
	decltype(m_stats.mp) m_max_mp;
};

}