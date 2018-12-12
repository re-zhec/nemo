#pragma once

#include <SFML/Graphics/RenderWindow.hpp>

namespace nemo
{

class GameObject;

class Graphics
{
public:
	virtual 
	~Graphics() 
	= default;

	virtual void 
	update(GameObject& obj, sf::RenderWindow& window) 
	= 0;
};

}