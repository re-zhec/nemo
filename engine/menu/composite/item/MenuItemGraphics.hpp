#pragma once

#include "../MenuEntryGraphics.hpp"

namespace nemo
{

class MenuItemGraphics : public MenuEntryGraphics
{
////////////////////////////////////////////////////////////////////////////////
//                                                                            //
////////////////////////////////////////////////////////////////////////////////
public:

	using MenuEntryGraphics::MenuEntryGraphics;

	/////////////////////////////////////////////////////
	//                                                 //
	/////////////////////////////////////////////////////

	MenuItemGraphics(
		const std::string&              file,
		const XYPair&                   pos,
		const XYPair&                   dim,
		const std::shared_ptr<sf::Font> font
	);

	virtual void 
	update(sf::RenderWindow& window) 
	override;

////////////////////////////////////////////////////////////////////////////////
//                                                                            //
////////////////////////////////////////////////////////////////////////////////
private:

	struct ParseInfo
	{
		XYPair        margins_;
		TextBoxColors colors_;
		int           font_sz_;
		bool          center_;

		ParseInfo(
			const XYPair&       margins = {XValue(-1.f), YValue(-1.f)},
			const TextBoxColors colors  = {{0,0,0},{0,0,0},{0,0,0}},
			const int           font_sz = 0,
			const bool          center  = false
		);
	};

	/////////////////////////////////////////////////////
	//                                                 //
	/////////////////////////////////////////////////////

	MenuItemGraphics(
		const ParseInfo&                info, 
		const XYPair&                   pos,
		const XYPair&                   dim,
		const std::shared_ptr<sf::Font> font
	);

	ParseInfo
	parse(const std::string& file)
	const;
};

}