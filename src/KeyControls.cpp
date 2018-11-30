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
	constexpr auto key_up     = "up";
	constexpr auto key_down   = "down";
	constexpr auto key_left   = "left";
	constexpr auto key_right  = "right";
	constexpr auto key_select = "select";
	constexpr auto key_cancel = "cancel";
	constexpr auto key_pause  = "pause";

	constexpr auto path = "json/settings/controls.json";
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
	ofs << std::setw(4) << js << std::endl;
}

////////////////////////////////////////////////////////////////////////////////
//                                                                            //
////////////////////////////////////////////////////////////////////////////////

void
KeyControls::load()
{
	// Load json file.
	std::ifstream ifs(path);

	if (ifs.fail()) {
		// Fail to read it. Set to default settings.
		set();
	}
	else {
		nlohmann::json js;
		ifs >> js;

		set(
			Up     { js[key_up]     }, 
			Down   { js[key_down]   }, 
			Left   { js[key_left]   },
			Right  { js[key_right]  },
			Select { js[key_select] },
			Cancel { js[key_cancel] },
			Pause  { js[key_pause]  }
		);
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