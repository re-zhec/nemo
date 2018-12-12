#pragma once

#include <memory>
#include <SFML/Graphics/RenderWindow.hpp>

#include "utility/type/Key.hpp"
#include "utility/type/XY.hpp"

namespace nemo
{

class Input;
class Physics;
class Graphics;

class GameObject
{
public:
	GameObject(
		const XYPair& pos,
		std::unique_ptr<Input>&& input,
		std::unique_ptr<Physics>&& physics,
		std::unique_ptr<Graphics>&& graphics
	);

	void
	update(sf::RenderWindow& window, const KeyAction action);

protected:
	XYPair pos_;
	std::unique_ptr<Input>    input_;
	std::unique_ptr<Physics>  physics_;
	std::unique_ptr<Graphics> graphics_;
};

}