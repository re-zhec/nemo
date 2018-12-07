#include <optional>

#include "utility/type/Key.hpp"
#include "Menu.hpp"
#include "MenuHandler.hpp"

namespace nemo
{

////////////////////////////////////////////////////////////////////////////////
//                                                                            //
////////////////////////////////////////////////////////////////////////////////

std::optional<int>
MenuHandler::handleAction(Menu& menu, const KeyAction action) 
{
	std::optional<int> selected = {};

	switch (action) {
		case KeyAction::Cancel:
			// BOOST_LOG_TRIVIAL(debug) << "Pressed cancel key";
			break;

		case KeyAction::Up:
			// BOOST_LOG_TRIVIAL(debug) << "Pressed up key";
			menu.moveUp();
			break;

		case KeyAction::Down:
			// BOOST_LOG_TRIVIAL(debug) << "Pressed down key.";
			menu.moveDown();
			break;

		case KeyAction::Left:
			// BOOST_LOG_TRIVIAL(debug) << "Pressed left key.";
			menu.moveLeft();
			break;

		case KeyAction::Right:
			// BOOST_LOG_TRIVIAL(debug) << "Pressed right key.";
			menu.moveRight();
			break;

		case KeyAction::Select:
			selected = menu.cursorAt();
			break;

		default:
			break;
	}

	return selected;
}

////////////////////////////////////////////////////////////////////////////////
//                                                                            //
////////////////////////////////////////////////////////////////////////////////

}