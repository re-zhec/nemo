#include <fstream>
#include <iomanip>
#include <boost/container/flat_map.hpp>
#include <SFML/Window/Keyboard.hpp>
#include <SFML/Window/Event.hpp>

#include "nlohmann/json.hpp"
#include "utility/type/Key.hpp"
#include "KeyControls.hpp"

namespace nemo
{

////////////////////////////////////////////////////////////////////////////////
//                                                                            //
////////////////////////////////////////////////////////////////////////////////

namespace {
	// If you want to change a key name, the path of the JSON file storing the 
	// configurations, or the indentation, then change the literals below.
	constexpr auto key_up     = "up";
	constexpr auto key_down   = "down";
	constexpr auto key_left   = "left";
	constexpr auto key_right  = "right";
	constexpr auto key_select = "select";
	constexpr auto key_cancel = "cancel";
	constexpr auto key_pause  = "pause";

	constexpr auto path = "data/settings/controls.json";
	constexpr auto indent = 4;
}

////////////////////////////////////////////////////////////////////////////////
//                                                                            //
////////////////////////////////////////////////////////////////////////////////

KeyControls::Up::Up         (const sf::Keyboard::Key code) : Key(code) {}
KeyControls::Down::Down     (const sf::Keyboard::Key code) : Key(code) {}
KeyControls::Left::Left     (const sf::Keyboard::Key code) : Key(code) {}
KeyControls::Right::Right   (const sf::Keyboard::Key code) : Key(code) {}
KeyControls::Select::Select (const sf::Keyboard::Key code) : Key(code) {}
KeyControls::Cancel::Cancel (const sf::Keyboard::Key code) : Key(code) {}
KeyControls::Pause::Pause   (const sf::Keyboard::Key code) : Key(code) {}

////////////////////////////////////////////////////////////////////////////////
//                                                                            //
////////////////////////////////////////////////////////////////////////////////

KeyControls::KeyControls()
	: map_(
		[](const Key a, const Key b) {
			return a < b;
		}
	)
{
	map_.reserve(static_cast<size_t>(KeyAction::count));
	load();
}

////////////////////////////////////////////////////////////////////////////////
//                                                                            //
////////////////////////////////////////////////////////////////////////////////

KeyControls::~KeyControls()
{
	save();
}

////////////////////////////////////////////////////////////////////////////////
//                                                                            //
////////////////////////////////////////////////////////////////////////////////

void
KeyControls::load()
{
	try {
		// Parse the JSON file.
		std::ifstream ifs(path);
		ifs.exceptions(std::ios::failbit | std::ios::badbit);
		nlohmann::json js;
		ifs >> js;

		set(
			Up     { js.at(key_up)     }, 
			Down   { js.at(key_down)   }, 
			Left   { js.at(key_left)   },
			Right  { js.at(key_right)  },
			Select { js.at(key_select) },
			Cancel { js.at(key_cancel) },
			Pause  { js.at(key_pause)  }
		);
	}
	catch (const std::ios_base::failure& e) {
		// Some error trying to read the file.
		// Use default controls.
		set();
	}
	catch (const nlohmann::json::out_of_range& e) {
		// Cannot find a key name in the file.
		// Use default controls.
		set();
	}
}

////////////////////////////////////////////////////////////////////////////////
//                                                                            //
////////////////////////////////////////////////////////////////////////////////

void
KeyControls::save() const
{
	nlohmann::json js;
	
	for (auto m : map_) {
		const auto [key, action] = m;

		// JSON content.
		switch (action) {
			case KeyAction::Up:     js[key_up]     = sf::Keyboard::Key(key); break;
			case KeyAction::Down:   js[key_down]   = sf::Keyboard::Key(key); break;
			case KeyAction::Left:   js[key_left]   = sf::Keyboard::Key(key); break;
			case KeyAction::Right:  js[key_right]  = sf::Keyboard::Key(key); break;
			case KeyAction::Select: js[key_select] = sf::Keyboard::Key(key); break;
			case KeyAction::Cancel: js[key_cancel] = sf::Keyboard::Key(key); break;
			case KeyAction::Pause:  js[key_pause]  = sf::Keyboard::Key(key); break;
			default: break;
		}
	};

	// Save it to the file.
	std::ofstream ofs(path);
	ofs << std::setw(indent) << js << std::endl;	
}

////////////////////////////////////////////////////////////////////////////////
//                                                                            //
////////////////////////////////////////////////////////////////////////////////

std::optional<KeyAction> 
KeyControls::convert(const sf::Event& event) 
const noexcept
{
	if (event.type != sf::Event::KeyPressed)
		return {};
	
	const auto it = map_.find(Key(event.key.code));

	if (it == map_.cend()) {
		// Not a registered key.
		return {};
	}

	[[maybe_unused]] const auto [UNUSED_, action] = *it;
	return { action };
}

////////////////////////////////////////////////////////////////////////////////
//                                                                            //
////////////////////////////////////////////////////////////////////////////////

void 
KeyControls::set(
	Up     up,
	Down   down,
	Left   left,
	Right  right,
	Select select,
	Cancel cancel,
	Pause  pause
) noexcept
{
	map_[up]     = KeyAction::Up;
	map_[down]   = KeyAction::Down;
	map_[left]   = KeyAction::Left;
	map_[right]  = KeyAction::Right;
	map_[select] = KeyAction::Select;
	map_[cancel] = KeyAction::Cancel;
	map_[pause]  = KeyAction::Pause;
}

}