#include <iostream>
#include <vector>
#include <thread>		//std::thread
#include <chrono>		//std::chrono_literals;
#include <filesystem>	//std::path

#define SOL_ALL_SAFETIES_ON 1
#include <sol/sol.hpp>
#include <efsw/efsw.hpp>

#include "FileWatchListener.h"
#include "Component.h"

using namespace std::chrono_literals;
namespace fs = std::filesystem;

fs::path const& getLuaScriptsDir()
{
	static fs::path luaScriptsDir;

	//Compute luaScriptsDir only once, then always return the same computed value
	if (luaScriptsDir.empty())
	{
		//current_path is somewhere in the project directory
		fs::path projectRoot = fs::current_path();

		//Rewind until the project root
		while (projectRoot.stem() != "CppLuaBindingGenerator")
		{
			projectRoot = projectRoot.parent_path();
		}

		luaScriptsDir = projectRoot / "Project" / "Tests" / "LuaScripts";
	}

	return luaScriptsDir;
}

fs::path getLuaScriptPath(std::string&& filename)
{
	return getLuaScriptsDir() / std::forward<std::string>(filename);
}

bool initLua(sol::state& lua)
{
	lua.open_libraries(sol::lib::base);

	//Must init components before loading scripts or an error will be thrown for unknown types
	bool result = Component::initLuaBinding(lua);

	//Add all lua scripts contained in LuaScripts/ folder to the state
	for (auto& entry : fs::recursive_directory_iterator(getLuaScriptsDir()))
	{
		if (entry.is_regular_file())
		{
			fs::path path = entry.path();

			if (path.has_extension() && path.extension() == ".lua")
			{
				lua.script_file(path.string());
			}
		}
	}

	return result;
}

bool deinitLua()
{
	return Component::deinitLuaBinding();
}

void initFileWatcher(sol::state& lua)
{
	static efsw::FileWatcher fileWatcher;
	static FileWatchListener fileListener(lua);

	efsw::WatchID id = fileWatcher.addWatch(getLuaScriptsDir().string(), &fileListener, true);

	std::cout << efsw::Errors::Log::getLastErrorLog() << std::endl;

	fileWatcher.watch();
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

	initFileWatcher(lua);

	loop(lua);

	if (!deinitLua())
	{
		std::cerr << "Failed to deinit lua." << std::endl;
	}

	return EXIT_SUCCESS;
}