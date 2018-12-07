#include "Item.hpp"

namespace nemo
{

Item::Item(int id)
	: m_id(id)
{
}


Item::~Item()
{
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