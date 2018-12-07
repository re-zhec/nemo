#pragma once

#include <functional>
#include <optional>
#include <boost/container/flat_map.hpp>
#include <SFML/Window/Keyboard.hpp>

#include "utility/type/Key.hpp"

namespace nemo
{

/**
 * \brief Configurations for the controls of the game.
 */
class KeyControls
{
public:
	/**
	 * \brief Construct the controls.
	 * 
	 * This constructor reads from a JSON file containing the keys mapped to 
	 * the controls. If there is an error reading the file for any reason e.g.
	 * the keys in the file are incorrectly named, the file doesn't exist, etc., 
	 * it loads the above public members with the game's default settings.
	 */
	KeyControls();

	/**
	 * \brief Destroy the controls.
	 * 
	 * Before the object is destroyed, it saves the key configurations to a JSON
	 * file that the constructor will read off of next time. If the file doesn't
	 * exist, it creates one.
	 */
	~KeyControls();
	
	/**
	 * \brief Set the controls to the key configurations found in the JSON file.
	 */
	void load();

	/**
	 * \brief Updates the JSON file with the current key configurations.
	 */
	void save() const;

	/**
	 * \brief Get the control associated with an user event.
	 * 
	 * For example, if the A, W, D, and S keys are mapped to the left, up, right, 
	 * and down. This method would return the left command for A being pressed, 
	 * up command for W being pressed, etc..
	 * 
	 * \param key   User event.
	 * 
	 * \return The control associated with that event. Or nothing if there is 
	 * none associated with it.
	 */
	std::optional<KeyAction> convert(const sf::Event& event) 
	const noexcept;

private:
	struct Up     : public Key { Up    (const sf::Keyboard::Key code); };
	struct Down   : public Key { Down  (const sf::Keyboard::Key code); };
	struct Left   : public Key { Left  (const sf::Keyboard::Key code); };
	struct Right  : public Key { Right (const sf::Keyboard::Key code); };
	struct Select : public Key { Select(const sf::Keyboard::Key code); };
	struct Cancel : public Key { Cancel(const sf::Keyboard::Key code); };
	struct Pause  : public Key { Pause (const sf::Keyboard::Key code); };

	///< All keys and what they are mapped to are stored here.
	boost::container::flat_map< Key, KeyAction, 
		std::function <bool (const Key&, const Key&)> 
	> map_;

	/**
	 * \brief Set the current controls of the game.
	 * 
	 * \param up        New up key setting.
	 * \param down      New down key setting.
	 * \param left      New left key setting.
	 * \param right     New right key setting.
	 * \param select    New select key setting.
	 * \param cancel    New cancel key setting.
	 * \param pause     New pause key setting.
	 */
	void set(
		Up     up     = Up     (sf::Keyboard::W),
		Down   down   = Down   (sf::Keyboard::S),
		Left   left   = Left   (sf::Keyboard::A),
		Right  right  = Right  (sf::Keyboard::D),
		Select select = Select (sf::Keyboard::P),
		Cancel cancel = Cancel (sf::Keyboard::O),
		Pause  pause  = Pause  (sf::Keyboard::Backspace)
	) noexcept;
};

}