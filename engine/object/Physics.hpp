#pragma once

namespace nemo
{

class GameObject;

class Physics
{
public:
	virtual 
	~Physics() = default;
	
	virtual void 
	update(GameObject& obj) = 0;
};

}