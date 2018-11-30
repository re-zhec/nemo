#pragma once
#include <SFML/Window/Keyboard.hpp>

namespace rp
{

/**
 * \brief Configurations for the controls of the game.
 */
class KeyControls
{
public:
	sf::Keyboard::Key up_;     ///< Up key.
	sf::Keyboard::Key down_;   ///< Down key.
	sf::Keyboard::Key left_;   ///< Left key.
	sf::Keyboard::Key right_;  ///< Right key.
	sf::Keyboard::Key select_; ///< Select key.
	sf::Keyboard::Key cancel_; ///< Cancel or back key.
	sf::Keyboard::Key pause_;  ///< Pause key.

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
	
	KeyControls(const KeyControls&)              = default;
	KeyControls(KeyControls&&)                   = default;
	KeyControls& operator=(const KeyControls&) & = default;
	KeyControls& operator=(KeyControls&&) &      = default;
	
	/**
	 * \brief Set the controls to the key configurations from the JSON file.
	 */
	void load();

	/**
	 * \brief Updates the JSON file with the current key configurations.
	 */
	void save() const;

private:
	struct Key { sf::Keyboard::Key v; };

	struct Up     : public Key {};
	struct Down   : public Key {};
	struct Left   : public Key {};
	struct Right  : public Key {};
	struct Select : public Key {};
	struct Cancel : public Key {};
	struct Pause  : public Key {};

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
		Up up         = Up     { sf::Keyboard::W },
		Down down     = Down   { sf::Keyboard::S },
		Left left     = Left   { sf::Keyboard::A },
		Right right   = Right  { sf::Keyboard::D },
		Select select = Select { sf::Keyboard::P },
		Cancel cancel = Cancel { sf::Keyboard::O },
		Pause pause   = Pause  { sf::Keyboard::Backspace }
	) noexcept;
};

}