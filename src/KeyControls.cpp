#include <fstream>
#include <iomanip>
#include <SFML/Window/Keyboard.hpp>
#include <nlohmann/json.hpp>
#include "../include/KeyControls.hpp"

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
KeyControls::save() const
{
	const nlohmann::json js = {
		{ key_up,     up_     },
		{ key_down,   down_   },
		{ key_left,   left_   },
		{ key_right,  right_  },
		{ key_select, select_ },
		{ key_cancel, cancel_ },
		{ key_pause,  pause_  }
	};

	// Save to json file.
	std::ofstream ofs(path);
	ofs << std::setw(indent) << js << std::endl;	
}

////////////////////////////////////////////////////////////////////////////////
//                                                                            //
////////////////////////////////////////////////////////////////////////////////

void
KeyControls::load()
{
	try {
		std::ifstream ifs(path);
		ifs.exceptions(
			// Handle different exceptions as 'file not found', 'permission 
			// denied'.
			ifs.exceptions()
			| std::ios_base::failbit
			| std::ios_base::badbit
		);

		// Read json file.
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
		set();
	}
	catch (const nlohmann::json::out_of_range& e) {
		set();
	}
}

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
	up_     = up.v;
	down_   = down.v;
	left_   = left.v;
	right_  = right.v;
	select_ = select.v;
	cancel_ = cancel.v;
	pause_  = pause.v;
}

}