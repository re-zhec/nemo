#include "../include/logger.hpp"
#include "../include/Item.hpp"

namespace rp
{

Item::Item(int id)
	: m_id(id)
{
	LOG_DEBUG("Creating item with id " << m_id);
}


Item::~Item()
{
	LOG_DEBUG("Destroying item id " << m_id);
}


int Item::ID() const noexcept
{
	return m_id;
}

std::string Item::Name() const
{
	return m_name;
}

}