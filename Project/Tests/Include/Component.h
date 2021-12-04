#pragma once

#include <CppLuaBind/LuaProperties.h>

#include "Generated/Component.h.h"

class CLASS(clb::LuaClass) Component
{
	private:
		FIELD(clb::LuaVar(clb::ReadOnly, "intReadonlyLua"))
		int intReadonly = 0;

		FIELD(clb::LuaVar(clb::ReadWrite, "intReadWriteLua"))
		int intReadWrite = 0;

		FIELD(clb::LuaVar) //Defaults to ReadWrite / c++ var name
		int defaultInt = 0;

	public:
		METHOD(clb::LuaFunc(clb::LuaImpl, "initlua"))
		virtual void init();

		METHOD(clb::LuaFunc(clb::LuaExposed))
		virtual void update();

		METHOD(clb::LuaFunc(clb::LuaExposed))
		static void staticFunc() noexcept;

		METHOD(clb::LuaFunc(clb::LuaImpl))
		static void staticFuncLua();

		METHOD(clb::LuaFunc) //Defaults to LuaExposed / c++ func name
		void defaultMethod();

/*
	////// TARGET GENERATED CODE FROM HERE

	private:
		static sol::usertype<Component> luaType;

	public:
		static bool initLuaBinding(sol::state& luaState);
		static bool deinitLuaBinding();

	////// END TARGET GENERATED CODE
*/

	Component_GENERATED
};

File_Component_GENERATED