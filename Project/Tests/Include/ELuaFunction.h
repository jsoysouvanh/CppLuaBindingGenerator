#pragma once

enum class ELueFunction
{
	/**
	*	Function is callable from lua scripts.
	*/
	ExposedToLua,

	/**
	*	Function is implemented in lua.
	*/
	LuaImplemented
};