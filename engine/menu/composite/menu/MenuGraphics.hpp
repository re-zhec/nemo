#pragma once

#include "utility/type/RowColumn.hpp"
#include "utility/RC1DConverter.hpp"
#include "../MenuEntryGraphics.hpp"

namespace nemo
{

class MenuGraphics : public MenuEntryGraphics
{
////////////////////////////////////////////////////////////////////////////////
//                                                                            //
////////////////////////////////////////////////////////////////////////////////
public:

	MenuGraphics(
		const XYPair&                   pos,
		const XYPair&                   dim,
		const XYPair&                   margins,
		const TextBoxColors             colors,
		const std::shared_ptr<sf::Font> font,
		const int                       font_sz,
		const bool                      center,
		const Row&                      rows,
		const Column&                   cols
	);

	MenuGraphics(
		const std::string&              file,
		const std::shared_ptr<sf::Font> font
	);

	virtual void 
	update(sf::RenderWindow& window) 
	override;

////////////////////////////////////////////////////////////////////////////////
//                                                                            //
////////////////////////////////////////////////////////////////////////////////
private:

	Row    rows_;
	Column cols_;

	/////////////////////////////////////////////////////
	//                                                 //
	/////////////////////////////////////////////////////

	struct ParseInfo
	{
		XYPair        pos_;
		XYPair        dim_;
		XYPair        margins_;
		TextBoxColors colors_;
		int           font_sz_;
		bool          center_;
		Row           rows_;
		Column        cols_;

		ParseInfo(
			const XYPair&       pos     = {XValue(-1.f), YValue(-1.f)},
			const XYPair&       dim     = {XValue(0.f), YValue(0.f)},
			const XYPair&       margins = {XValue(-1.f), YValue(-1.f)},
			const TextBoxColors colors  = {{0,0,0},{0,0,0},{0,0,0}},
			const int           font_sz = 0,
			const bool          center  = false,
			const Row&          rows    = Row(0),
			const Column&       cols    = Column(0)
		);
	};

	/////////////////////////////////////////////////////
	//                                                 //
	/////////////////////////////////////////////////////

	MenuGraphics(
		const ParseInfo&                info,
		const std::shared_ptr<sf::Font> font
	);

	ParseInfo
	parse(const std::string& file)
	const;
};

}