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

		luaType.set("initlua", &Component::init);
		luaType.set("update", &Component::update);

		return true;
	}

	return false;
}

bool Component::deinitLuaBinding()
{
	if (luaType.lua_state() != nullptr)
	{
		luaType.unregister();
		luaType = sol::usertype<Component>();	//crash if this line is not here, wtf

		return true;
	}

	return false;
}

//=======================================
//=======================================
//=======================================

#include <iostream>

void Component::init()
{
	std::cout << "Component::init() from C++" << std::endl;
}

void Component::update()
{
	std::cout << "Component::update() from C++" << std::endl;
}

