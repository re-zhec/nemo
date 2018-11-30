#pragma once
#include <SFML/Window.hpp>
#include "Menu.hpp"

namespace rp
{

class MenuCommand
{
public:	
	virtual void execute(Menu& menu) = 0;
};

class MenuUpCommand : public MenuCommand
{
public:
	virtual void execute(Menu& menu) override;
};

class MenuDownCommand : public MenuCommand
{
public:
	virtual void execute(Menu& menu) override;
};

class MenuRightCommand : public MenuCommand
{
public:
	virtual void execute(Menu& menu) override;
};

class MenuLeftCommand : public MenuCommand
{
public:
	virtual void execute(Menu& menu) override;
};

}