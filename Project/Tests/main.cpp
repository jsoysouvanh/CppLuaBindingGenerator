#include <iostream>
#include <vector>
#include <thread>		//std::thread
#include <chrono>		//std::chrono_literals;
#include <filesystem>	//std::path

#define SOL_ALL_SAFETIES_ON 1
#include <sol/sol.hpp>

#include "Component.h"

using namespace std::chrono_literals;
namespace fs = std::filesystem;

fs::path getLuaScriptPath(std::string&& filename)
{
	//current_path is somewhere in the project directory
	fs::path projectRoot = fs::current_path();

	//Rewind until the project root
	while (projectRoot.stem() != "CppLuaBindingGenerator")
	{
		projectRoot = projectRoot.parent_path();
	}

	return projectRoot / "Project" / "Tests" / "LuaScripts" / std::forward<std::string>(filename);
}

bool initLua(sol::state& lua)
{
	lua.open_libraries(sol::lib::base);

	return Component::initLuaBinding(lua);
}

bool deinitLua()
{
	return Component::deinitLuaBinding();
}

void reloadLuaScripts(sol::state& lua)
{
	static char const* luaScripts[] =
	{
		"Component.lua"
	};

	for (char const* luaScript : luaScripts)
	{
		lua.script_file(getLuaScriptPath(luaScript).string());
	}
}

void loop(sol::state& lua)
{
#ifdef CI_BUILD
	constexpr std::size_t const MAX_LOOP_COUNT = 5u;
#else
	constexpr std::size_t const MAX_LOOP_COUNT = 1000u;
#endif

	Component component;

	std::vector<Component*> components = { &component };

	std::size_t loopCount = 0u;
	std::string input;

#ifndef CI_BUILD
	std::thread inputPolling([&input]()
							 {
								 //Data race with main thread but we don't care
								 std::cin >> input;
							 }); 
#endif

	do
	{
		reloadLuaScripts(lua);

		std::cout << "=================" << std::endl << "BEGIN FRAME" << std::endl;

		component.init();
		component.update();

		Component::staticFuncLua();

		std::cout << std::endl;
		std::this_thread::sleep_for(1s);

	} while (input.empty() && loopCount++ <= MAX_LOOP_COUNT);

#ifndef CI_BUILD
	inputPolling.join();
#endif
}

int main()
{
	sol::state lua;

	if (!initLua(lua))
	{
		std::cerr << "Failed to init lua." << std::endl;
	}

	loop(lua);

	if (!deinitLua())
	{
		std::cerr << "Failed to deinit lua." << std::endl;
	}

	return EXIT_SUCCESS;
}