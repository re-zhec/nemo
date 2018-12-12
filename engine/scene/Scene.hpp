#pragma once

#include <memory>
#include <SFML/Graphics/RenderWindow.hpp>

namespace nemo
{

class Scene
{
public:
	virtual std::unique_ptr<Scene>
	update(sf::RenderWindow& window)
	const = 0;
};

}