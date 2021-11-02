#include <iostream>

#define SOL_ALL_SAFETIES_ON 1
#include <sol/sol.hpp>

#include "Component.h"

sol::state lua;

bool initLua()
{
	lua.open_libraries(sol::lib::base);

	return Component::initLuaBinding(lua);
}

bool deinitLua()
{
	return Component::deinitLuaBinding();
}

int main()
{
	if (!initLua())
	{
		std::cerr << "Failed to init lua." << std::endl;
	}

	//Do stuff here

	

	if (!deinitLua())
	{
		std::cerr << "Failed to deinit lua." << std::endl;
	}

	return EXIT_SUCCESS;
}