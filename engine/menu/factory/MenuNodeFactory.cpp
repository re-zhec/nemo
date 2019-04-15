#include <boost/assert.hpp>
#include <iostream>
#include <fstream>

#include "nlohmann/json.hpp"

#include "MenuNodeFactory.hpp"
#include "../composite/MenuTree.hpp"
#include "../composite/MenuLeaf.hpp"

namespace nemo
{

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////

void
MenuNodeFactory::setDefaultConfig(const std::string& file)
{
	config_default_ = parse(file);
}

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////

std::shared_ptr<MenuNode>
MenuNodeFactory::create(const MenuNodeType type, const std::string& file) 
const
{
	std::shared_ptr<MenuNode> entry = nullptr;
	Config config = file.empty() ? config_default_ : parse(file);

	switch (type) {
		case MenuNodeType::Tree:
			entry = std::make_shared<MenuTree>(
				config.pos_,
				config.dim_,
				config.row_by_col_,
				config.padding_,
				config.spacing_,
				config.box_colors_,
				config.entry_colors_,
				config.hover_colors_,
				config.font_
			);
			break;
		case MenuNodeType::Leaf:
			entry = std::make_shared<MenuLeaf>(
				config.pos_,
				config.dim_,
				config.padding_,
				config.entry_colors_,
				config.font_
			);
			break;
		default:
			break;
	}

	return entry;
}

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////

MenuNodeFactory::Config
MenuNodeFactory::parse(const std::string& file)
const
{
	std::ifstream ifs(file);
	BOOST_ASSERT(ifs);
	nlohmann::json js;
	ifs >> js;

	try {
		const auto js_pos = js.at("position");
		const auto pos = XYPair(
			XValue(js_pos.at("x")), 
			YValue(js_pos.at("y"))
		);

		const auto js_dim = js.at("dimensions");
		const auto dim = XYPair(
			XValue(js_dim.at("width")), 
			YValue(js_dim.at("height"))
		);

		const auto js_padding = js.at("padding");
		const auto padding = XYPair(
			XValue(js_padding.at("horizontal")),
			YValue(js_padding.at("vertical"))
		);

		const auto js_box_colors = js.at("colors");
		const auto box_colors = TextBoxColors(
			BorderColor    { sfMakeColor(js_box_colors.at("border")) },
			BackgroundColor{ sfMakeColor(js_box_colors.at("background")) },
			TextColor      { sfMakeColor(js_box_colors.at("text")) }
		);

		const auto js_font = js.at("font");
		const auto js_alignment = js_font.at("alignment") == "left" 
			? Alignment::Left
			: js_font.at("alignment") == "right"
				? Alignment::Right
				: Alignment::Center;
		
		const auto font = FontProperties(
			std::make_shared<sf::Font>(),
			js_font.at("size"),
			js_alignment
		);
		
		BOOST_VERIFY(font.family_->loadFromFile(js_font.at("family")));

		const auto row_by_col = RCPair(
			Row(js.at("rows")),
			Column(js.at("columns"))
		);
		
		const auto js_entry = js.at("entry");
		const auto js_entry_spacing = js_entry.at("spacing");
		const auto entry_spacing = XYPair(
			XValue(js_entry_spacing.at("horizontal")),
			YValue(js_entry_spacing.at("vertical"))
		);

		const auto js_entry_colors = js_entry.at("colors");
		const auto js_entry_colors_normal = js_entry_colors.at("normal");
		const auto entry_colors_normal = TextBoxColors(
			BorderColor    { sfMakeColor(js_entry_colors_normal.at("border")) },
			BackgroundColor{ sfMakeColor(js_entry_colors_normal.at("background")) },
			TextColor      { sfMakeColor(js_entry_colors_normal.at("text")) }
		);

		const auto js_entry_colors_hover = js_entry_colors.at("hover");
		const auto entry_colors_hover = TextBoxColors(
			BorderColor    { sfMakeColor(js_entry_colors_hover.at("border")) },
			BackgroundColor{ sfMakeColor(js_entry_colors_hover.at("background")) },
			TextColor      { sfMakeColor(js_entry_colors_hover.at("text")) }
		);

		return Config(
			pos,
			dim,
			padding,
			entry_spacing,
			font,
			box_colors,
			entry_colors_normal,
			entry_colors_hover,
			row_by_col
		);
	}
	catch (const nlohmann::json::out_of_range& e) {
		std::cout << "failed parsing " << file << ": " << e.what() << std::endl;
		return Config();
	}
}

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////

}