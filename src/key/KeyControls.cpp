#include <fstream>
#include <iomanip>
#include <boost/container/flat_map.hpp>
#include <SFML/Window/Keyboard.hpp>
#include <nlohmann/json.hpp>

#include "Key.hpp"
#include "KeyControls.hpp"

namespace rp
{

////////////////////////////////////////////////////////////////////////////////
//                                                                            //
////////////////////////////////////////////////////////////////////////////////

namespace {
	// If you want to change a key name, the path of the key controls json 
	// file, or the indentation, then change the literals below.
	constexpr auto key_up     = "up";
	constexpr auto key_down   = "down";
	constexpr auto key_left   = "left";
	constexpr auto key_right  = "right";
	constexpr auto key_select = "select";
	constexpr auto key_cancel = "cancel";
	constexpr auto key_pause  = "pause";

	constexpr auto path = "json/settings/controls.json";
	constexpr auto indent = 4;
}

////////////////////////////////////////////////////////////////////////////////
//                                                                            //
////////////////////////////////////////////////////////////////////////////////

KeyControls::KeyControls()
	: map_(
		[](const Key& a, const Key& b) {
			return a.v_ < b.v_;
		}
	)
{
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
		// Use default controls.
		set();
	}
	catch (const nlohmann::json::out_of_range& e) {
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
	
	for (auto& m : map_) {
		const auto [key, action] = m;

		// JSON content.
		switch (action) {
			case KeyAction::Up:     js[key_up]     = key.v_; break;
			case KeyAction::Down:   js[key_down]   = key.v_; break;
			case KeyAction::Left:   js[key_left]   = key.v_; break;
			case KeyAction::Right:  js[key_right]  = key.v_; break;
			case KeyAction::Select: js[key_select] = key.v_; break;
			case KeyAction::Cancel: js[key_cancel] = key.v_; break;
			case KeyAction::Pause:  js[key_pause]  = key.v_; break;
			default: break;
		}
	};

	// Save to file.
	std::ofstream ofs(path);
	ofs << std::setw(indent) << js << std::endl;	
}

////////////////////////////////////////////////////////////////////////////////
//                                                                            //
////////////////////////////////////////////////////////////////////////////////

std::optional<KeyAction> 
KeyControls::convert(const Key key) const noexcept
{
	const auto it = map_.find(key);

	if (it == map_.cend()) {
		return {};
	}

	const auto [UNUSED_, action] = *it;
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