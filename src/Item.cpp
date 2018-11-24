#include "../include/logger.hpp"
#include "../include/Item.hpp"
#include "../include/PartyChar.hpp"

namespace rp
{

Item::Item(ItemID id)
	: m_id(id)
{
	LOG_DEBUG("Creating item with id " << m_id);
}


Item::~Item()
{
	LOG_DEBUG("Destroying item id " << m_id);
}


ItemID Item::ID() const
{
	return m_id;
}


CharID Item::GetOwner() const
{
	return m_owner;
}


bool Item::SetOwner(CharID owner) &
{
	if (m_owner != CharID::None && owner != CharID::None) {
		// Already assigned to a party character
		LOG_DEBUG(
			"Cannot assign item id " << m_id << " to new owner id " << owner << 
			". Currently belongs to owner id " << m_owner
		);
		return false;
	}

	m_owner = owner;
	LOG_DEBUG("Assigned item id " << m_id << " to new owner id " << m_owner);
	return true;
}

}