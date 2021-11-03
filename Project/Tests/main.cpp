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
	//Working directory is CppLuaBindingGenerator/Build/x64-Debug/Bin
	return fs::current_path().parent_path().parent_path().parent_path() / "Project" / "Tests" / "LuaScripts" / std::forward<std::string>(filename);
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
	Component component;

	std::vector<Component*> components = { &component };

	std::string input;
	std::thread inputPolling([&input]()
							 {
								 //Data race with main thread but we don't care
								 std::cin >> input;
							 });

	do
	{
		reloadLuaScripts(lua);

		std::cout << "=================" << std::endl << "BEGIN FRAME" << std::endl;

		component.init();
		component.update();

		Component::staticFuncLua();

		std::cout << std::endl;
		std::this_thread::sleep_for(1s);

	} while (input.empty());

	inputPolling.join();
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