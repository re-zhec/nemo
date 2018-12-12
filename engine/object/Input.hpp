#pragma once

#include "utility/type/Key.hpp"

namespace nemo
{

class GameObject;

class Input
{
public:
	virtual 
	~Input() = 0;
	
	virtual void 
	update(GameObject& obj, const KeyAction action) = 0;
};

}