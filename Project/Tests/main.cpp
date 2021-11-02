#include <iostream>

#define SOL_ALL_SAFETIES_ON 1
#include <sol/sol.hpp>

int main()
{
	sol::state lua;

	lua.open_libraries(sol::lib::base);

	lua.script("print('bark bark bark!')");

	return EXIT_SUCCESS;
}