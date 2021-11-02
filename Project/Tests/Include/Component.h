#pragma once

#include <sol/sol.hpp>	//GENERATED CODE

#include "ELuaFunction.h"

#include "Generated/Component.h.h"

class CLASS() Component
{
	public:
		METHOD(LuaFunc(ELueFunction::ExposedToLua, "initlua"))
		virtual void init();

		METHOD(LuaFunc(ELueFunction::ExposedToLua, "update"))
		virtual void update();

		////// TARGET GENERATED CODE FROM HERE
	private:
		static sol::usertype<Component> luaType;

	public:
		static bool initLuaBinding(sol::state& luaState);
		static bool deinitLuaBinding();

		////// END TARGET GENERATED CODE

	Component_GENERATED
};

File_Component_GENERATED