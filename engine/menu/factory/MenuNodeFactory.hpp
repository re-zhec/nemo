#pragma once

#include <memory>
#include <string>

#include "../composite/MenuNode.hpp"
#include "utility/type/XY.hpp"
#include "utility/type/RowColumn.hpp"
#include "utility/type/Color.hpp"
#include "utility/type/FontProperties.hpp"
#include "utility/wrapper/sfMakeColor.hpp"

namespace nemo
{

/***
 * @brief Enumeration for the type of menu entry to create.
 ***/
enum class MenuNodeType {
	Leaf, ///< An item in a menu.
	Tree  ///< Menu or submenu.
};

class MenuNodeFactory
{
public:
	/***
	 * @brief Use menu configurations found in a file as the default for further 
	 * menu entry creations.
	 * 
	 * @param file        - Path to configuration file.
	 ***/
	void
	setDefaultConfig(const std::string& file);

	/***
	 * @brief Create a menu entry using either a configuration file or the 
	 * default configurations.
	 * 
	 * @param type        - Type of menu entry to create i.e. menu or menu item.
	 * @param file        - Configuation file to use. If unspecified, use default
	 *                    - configurations.
	 * 
	 * @return The created menu entry.
	 ***/
	std::shared_ptr<MenuNode>
	create(const MenuNodeType type, const std::string& file = "")
	const;

private:
	/***
	 * @brief MenuTree configurations to be extracted from a configuation file.
	 ***/
	struct Config
	{
		///< Top left position in the render window.
		XYPair         pos_;
		///< Overall size, including padding.
		XYPair         dim_;
		///< Horizontal and vertical padding at the border.
		XYPair         padding_;
		///< Horizontal and vertical margins between entries.
		XYPair         spacing_;
		///< Default font properties.
		FontProperties font_;
		///< Default textbox color set for the menu box.
		TextBoxColors  box_colors_;
		///< Default textbox color set for each entry.   
		TextBoxColors  entry_colors_;
		///< Default textbox color set for the entry the cursor is currently over.
		TextBoxColors  hover_colors_;
		///< Number of rows and columns of entries that can be displayed at a time.
		RCPair         row_by_col_;    

		/***
		 * @brief Constructor to populate parse fields. Refer to the public 
		 * attributes for parameters.
		 ***/
		Config(
			const XYPair& pos     = { XValue(-1.f), YValue(-1.f) },
			const XYPair& dim     = { XValue(0.f),  YValue(0.f)  },
			const XYPair& padding = { XValue(-1.f), YValue(-1.f) },
			const XYPair& spacing = { XValue(-1.f), YValue(-1.f) },
			const FontProperties& font = {
				static_cast<std::shared_ptr<sf::Font>>(nullptr), 
				0, 
				Alignment::Left
			},
			const TextBoxColors box_colors    = {
				BorderColor    { sfMakeColor({0,0,0,0}) },
				BackgroundColor{ sfMakeColor({0,0,0,0}) },
				TextColor      { sfMakeColor({0,0,0,0}) }
			},
			const TextBoxColors entry_colors  = {
				BorderColor    { sfMakeColor({0,0,0,0}) },
				BackgroundColor{ sfMakeColor({0,0,0,0}) },
				TextColor      { sfMakeColor({0,0,0,0}) }
			},
			const TextBoxColors hover_colors = {
				BorderColor    { sfMakeColor({0,0,0,0}) },
				BackgroundColor{ sfMakeColor({0,0,0,0}) },
				TextColor      { sfMakeColor({0,0,0,0}) }
			},
			const RCPair& row_by_col = { Row(0), Column(0) }
		)
			: pos_         (pos)
			, dim_         (dim)
			, padding_     (padding)
			, spacing_     (spacing)
			, font_        (font)
			, box_colors_  (box_colors)
			, entry_colors_(entry_colors)
			, hover_colors_(hover_colors)
			, row_by_col_  (row_by_col)
		{
		}
	};

	/***
	 * @brief Parse a menu configuration file.
	 * 
	 * @param file        - Path to configuration file.
	 * 
	 * @return Extracted menu configurations.
	 ***/
	Config
	parse(const std::string& file)
	const;

	///< Default menu configurations.
	Config config_default_;
};

}