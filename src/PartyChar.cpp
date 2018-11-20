#include <cassert>
#include <algorithm>
#include "../include/logger.hpp"
#include "../include/PartyChar.hpp"
#include "../include/Weapon.hpp"

namespace fe
{

PartyChar::PartyChar(CharID id)
	: m_id(id)
{
	// load stats with its match from the party character archive
	auto it = m_db.find(id);
	assert(it != m_db.cend());
	m_stats = it->second;

	LOG_DEBUG("Constructed party character with id " << m_id);
}


PartyChar::~PartyChar()
{
	LOG_DEBUG("Destroyed char id " << m_id);
}


CharID PartyChar::ID() const
{
	return m_id;
}


PartyChar::CharStats PartyChar::Stats() const
{
	return m_stats;
}


bool PartyChar::Equip(std::shared_ptr<Weapon> w) &
{
	// if (m_weaptype != w->Stats().type) {
	// 	// Incompatible weapon type
	// 	LOG_DEBUG(
	// 		"Char id " << m_id << " cannot wield this type of weapon. " << 
	// 		"Item id " << w->ID() << " is type " << w->Stats().type
	// 	);
	// 	return false;
	// }

	if (m_level < w->Stats().level) {
		// Character's level isn't high enough
		LOG_DEBUG(
			"Char id " << m_id << "'s level isn't high enough to equip " << 
			"item id " << w->ID() << ": " << m_level << " < " << w->Stats().level
		);
		return false;
	}

	// Assign this character as the weapon's owner 
	if (!w->SetOwner(m_id)) {
		// SetOwner() fails. Weapon is already owned by someone else
		return false;
	}

	m_weap = w;
	return true;
}

/**
 * PartyChar::Unequip
 * 	params:
 * 		- weapon slot
 * 	return: none
 * 
 * unequip weapon to unit
 */
void PartyChar::Unequip() &
{
	// Assign CharID::None to remove ownership over weapon. This must be done 
	// before freeing the pointer
	m_weap->SetOwner(CharID::None);
	m_weap.reset();
}

/**
 * PartyChar::Hurt
 * 	params:
 * 		- # of hit points to subtract
 * 	return: none
 * 
 * reduce unit hp
 */
void PartyChar::Hurt(int dmg) &
{
	// damage
	assert(dmg >= 0);
	m_stats.hp = std::max(0, m_stats.hp - dmg);
}

/**
 * PartyChar::Hurt
 * 	params:
 * 		- # of hit points to add
 * 	return: none
 * 
 * increase unit hp
 */
void PartyChar::Heal(int amt) &
{
	// heal
	assert(amt >= 0);
	m_stats.hp = std::max(m_cur_hp, m_stats.hp + amt);
}

// battle unit database
std::unordered_map<CharID, PartyChar::CharStats> PartyChar::m_db = 
{
	{ CharID::Chrom, {"Chrom", 20,  5, 15, 15, 10, 10, 10, 1} },
	{ CharID::Robin, {"Robin", 15, 10, 10, 10, 10, 10, 15, 1} },
};

}