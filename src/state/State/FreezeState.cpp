#include <memory>
#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Window/Event.hpp>

#include "../State.hpp"
#include "FreezeState.hpp"

namespace nemo
{

std::unique_ptr<State> 
FreezeState::handleEvent(const sf::Event& event)
{
	return nullptr;
}

void 
FreezeState::update(sf::RenderWindow& window)
{
}

}