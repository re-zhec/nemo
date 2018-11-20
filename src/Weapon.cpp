#include <cassert>
#include <limits>
#include "../include/Weapon.hpp"
#include "../include/PartyChar.hpp"

#define COMBAT_CLOSE_ONLY		{1, 1}
#define COMBAT_CLOSE_RANGED	{1, 2}
#define COMBAT_RANGED_ONLY		{2, 2}

namespace fe
{

Weapon::Weapon(ItemID id)
	: Item(id)
{
	// load stats with its match from archive
	auto it = m_db.find(id);
	assert(it != m_db.cend());	
	
	m_stats = it->second;
	assert(m_stats.level >= 1 && m_stats.level <= 99);
	assert(m_stats.might >= 0);
	assert(m_stats.hit >= 0 && m_stats.hit <= 100);
	assert(m_stats.crit >= 0 && m_stats.crit <= 100);
	assert(m_stats.range.first >= 1 && m_stats.range.second >= 1);
	assert(m_stats.uses >= 0);
}


std::string Weapon::Name() const
{
	return m_stats.name;
}


Weapon::WeapStats Weapon::Stats() const
{
	return m_stats;
}


bool Weapon::IsUsable() const
{
	return m_stats.uses == 0;
}


void Weapon::Tear()
{
	if (m_stats.uses > 0) {
		--m_stats.uses;
	}
}

void Weapon::Restore(const decltype(WeapStats::uses) uses)
{
	assert(uses >= 0);

	// several weapons can be used infinitely
	// infinity weapon value is represented as max value for data type
	auto inf = std::numeric_limits<decltype(WeapStats::uses)>::max();

	// anything less than the max value has finite uses
	if (m_stats.uses < inf) {
		// take care not to overflow or reach max value (infinity)
		if (inf - m_stats.uses >= uses) {
			m_stats.uses = inf - 1;
		}
		else {
			m_stats.uses += uses;
		}
	}
}

// Weapon archive
std::unordered_map<ItemID, Weapon::WeapStats> Weapon::m_db = 
{
	{ ItemID::BrzSwd, {"Bronze Sword", WeapType::Sword,  1,  3, 90, 0, COMBAT_CLOSE_ONLY, 50} },
	{ ItemID::IrnSwd, {"Iron Sword",   WeapType::Sword,  5,  6, 85, 0, COMBAT_CLOSE_ONLY, 40} },
	{ ItemID::SteSwd, {"Steel Sword",  WeapType::Sword, 10,  9, 80, 0, COMBAT_CLOSE_ONLY, 35} },
	{ ItemID::SlvSwd, {"Silver Sword", WeapType::Sword, 11, 13, 75, 0, COMBAT_CLOSE_ONLY, 30} },

	{ ItemID::BrzAxe, {"Bronze Axe", WeapType::Axe,  1,  4, 80, 0, COMBAT_CLOSE_ONLY, 50} },
	{ ItemID::IrnAxe, {"Iron Axe",   WeapType::Axe,  5,  7, 75, 0, COMBAT_CLOSE_ONLY, 40} },
	{ ItemID::SteAxe, {"Steel Axe",  WeapType::Axe, 10, 11, 70, 0, COMBAT_CLOSE_ONLY, 35} },
	{ ItemID::SlvAxe, {"Silver Axe", WeapType::Axe, 15, 15, 65, 0, COMBAT_CLOSE_ONLY, 30} },

	{ ItemID::BrzLan, {"Bronze Lance", WeapType::Lance,  1,  3, 90, 0, COMBAT_CLOSE_ONLY, 50} },
	{ ItemID::IrnLan, {"Iron Lance",   WeapType::Lance,  5,  6, 85, 0, COMBAT_CLOSE_ONLY, 40} },
	{ ItemID::SteLan, {"Steel Lance",  WeapType::Lance, 10,  9, 80, 0, COMBAT_CLOSE_ONLY, 35} },
	{ ItemID::SlvLan, {"Silver Lance", WeapType::Lance, 15, 13, 75, 0, COMBAT_CLOSE_ONLY, 30} },

	{ ItemID::Fire,    {"Fire",      WeapType::Fire,  1,  2, 90, 0, COMBAT_CLOSE_RANGED, 45} },
	{ ItemID::Elfire,  {"Elfire",    WeapType::Fire,  5,  5, 85, 0, COMBAT_CLOSE_RANGED, 35} },
	{ ItemID::Arcfire, {"Arcfire",   WeapType::Fire, 10,  8, 80, 0, COMBAT_CLOSE_RANGED, 30} },
	{ ItemID::Bolgano, {"Bolganone", WeapType::Fire, 15, 12, 75, 0, COMBAT_CLOSE_RANGED, 25} },
};

}