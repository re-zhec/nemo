#include <SFML/Window/Keyboard.hpp>

namespace rp
{

class KeyControls
{
public:
	sf::Keyboard::Key up_;
	sf::Keyboard::Key down_;
	sf::Keyboard::Key left_;
	sf::Keyboard::Key right_;
	sf::Keyboard::Key select_;
	sf::Keyboard::Key cancel_;
	sf::Keyboard::Key pause_;

	KeyControls();
	~KeyControls();

	void load();
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