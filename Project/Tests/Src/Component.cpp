#include "Generated/Component.cpp.h"

//=======================================
//=========== GENERATED CODE ============
//=======================================

sol::usertype<Component> Component::luaType;

bool Component::initLuaBinding(sol::state& luaState)
{
	if (luaType.lua_state() == nullptr)
	{
		luaType = luaState.new_usertype<Component>("Component");

		//All fields
		luaType.set("intReadonlyLua", sol::readonly(&Component::intReadonly));
		luaType.set("intReadWriteLua", &Component::intReadWrite);

		//All methods that are LuaExposed
		luaType.set("update", &Component::update);

		//All static methods that are LuaExposed
		luaType.set("staticFunc", &Component::staticFunc);

		return true;
	}

	return false;
}

bool Component::deinitLuaBinding()
{
	if (luaType.lua_state() != nullptr)
	{
		luaType.unregister();
		luaType = sol::usertype<Component>();	//sol::state dtor crash if this line is not here, wtf

		return true;
	}

	return false;
}

//Generated from LuaFunc(LuaImpl, "initlua") property
void Component::init()
{
	//TODO: Should maybe forward the call to an external class to handle
	//call status
	luaType["initlua"].call(this);
}

//Generated from LuaFunc(staticFuncLua) property
void Component::staticFuncLua() noexcept
{
	luaType["staticFuncLua"].call();
}

//=======================================
//=======================================
//=======================================

#include <iostream>

void Component::update()
{
	std::cout << "Component::update() from C++" << std::endl;
}

void Component::staticFunc() noexcept
{
	std::cout << "Component::staticFunc() from C++" << std::endl;
}