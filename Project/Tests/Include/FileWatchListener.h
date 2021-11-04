#pragma once

#include <iostream>
#include <filesystem>

#include <sol/sol.hpp>
#include <efsw/efsw.hpp>

class FileWatchListener : public efsw::FileWatchListener
{
	private:
		//Reload scripts in the lua state when they change
		sol::state& _lua;

		static bool isLuaScript(std::filesystem::path const& file)
		{
			return file.has_extension() && file.extension() == ".lua";
		}

		void reloadScript(std::filesystem::path script)
		{
			if (isLuaScript(script))
			{
				std::cout << "Reload script: " << script.string() << std::endl;
				_lua.script_file(script.string());
			}
		}

		void removeScript(std::filesystem::path script)
		{
			if (isLuaScript(script))
			{
				std::cout << "Remove script: " << script.string() << std::endl;

				//Remove the script
				_lua[script.string()] = sol::lua_nil;
			}
		}

		void renameScript(std::filesystem::path oldScript, std::filesystem::path newScript)
		{
			std::cout << newScript << " has event Moved from " << oldScript << std::endl;

			if (isLuaScript(oldScript))
			{
				removeScript(std::move(oldScript));
			}

			if (isLuaScript(newScript))
			{
				reloadScript(std::move(newScript));
			}
		}

		virtual void handleFileAction(efsw::WatchID watchid, std::string const& dir,
									  std::string const& filename, efsw::Action action, std::string oldFilename) override
		{
			switch (action)
			{
				case efsw::Actions::Add:
					[[fallthrough]];
				case efsw::Actions::Modified:
					reloadScript(std::filesystem::path(dir) / filename);
					break;

				case efsw::Actions::Delete:
					removeScript(std::filesystem::path(dir) / filename);
					std::cout << "DIR (" << dir << ") FILE (" << filename << ") has event Delete" << std::endl;
					break;
				
				case efsw::Actions::Moved:
					renameScript(std::filesystem::path(dir) / oldFilename, std::filesystem::path(dir) / filename);
					break;

				default:
					std::cout << "Should never happen!" << std::endl;
			}
		}

	public:
		FileWatchListener(sol::state& lua) noexcept:
			_lua{lua}
		{}
};