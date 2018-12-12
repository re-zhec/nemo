#pragma once

namespace nemo
{

class MenuItem;
class Menu;

class MenuCursor
{
public:

	void
	visit(Menu& menu);

	void
	visit(MenuItem& menu);

private:
};

}