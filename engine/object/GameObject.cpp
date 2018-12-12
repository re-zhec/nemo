#include <utility>

#include "GameObject.hpp"
#include "Input.hpp"
#include "Physics.hpp"
#include "Graphics.hpp"

namespace nemo
{

////////////////////////////////////////////////////////////////////////////////
//                                                                            //
////////////////////////////////////////////////////////////////////////////////

GameObject::GameObject(
	const XYPair& pos,
	std::unique_ptr<Input>&& input,
	std::unique_ptr<Physics>&& physics,
	std::unique_ptr<Graphics>&& graphics
)
	: pos_(pos)
	, input_(std::move(input))
	, physics_(std::move(physics))
	, graphics_(std::move(graphics))
{
}

////////////////////////////////////////////////////////////////////////////////
//                                                                            //
////////////////////////////////////////////////////////////////////////////////

void
GameObject::update(sf::RenderWindow& window, const KeyAction action)
{
	if (input_ != nullptr)
		input_->update(*this, action);
	
	if (physics_ != nullptr) 
		physics_->update(*this);
	
	if (graphics_ != nullptr)
		graphics_->update(*this, window);	
}

////////////////////////////////////////////////////////////////////////////////
//                                                                            //
////////////////////////////////////////////////////////////////////////////////

}