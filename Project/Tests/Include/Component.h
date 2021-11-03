#pragma once

#include <sol/sol.hpp>	//GENERATED CODE

#include "ELuaProperty.h"

#include "Generated/Component.h.h"

class CLASS() Component
{
	private:
		FIELD(LuaVar(ReadOnly, "intReadonlyLua"))
		int intReadonly = 0;

		FIELD(LuaVar(ReadWrite, "intReadWriteLua"))
		int intReadWrite = 0;

	public:
		METHOD(LuaFunc(LuaImpl, "initlua"))
		virtual void init();

		METHOD(LuaFunc(LuaExposed))
		virtual void update();

		METHOD(LuaFunc(LuaExposed))
		static void staticFunc() noexcept;

		METHOD(LuaFunc(LuaImpl))
		static void staticFuncLua() noexcept;





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