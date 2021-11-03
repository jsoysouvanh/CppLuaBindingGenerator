#pragma once

enum ELuaProperty
{
	/**
	*	Function is implemented in c++ and
	*	callable from both lua and c++.
	*/
	LuaExposed,

	/**
	*	Function is implemented in lua and
	*	callable from both lua and c++.
	*/
	LuaImpl,

	/**
	*	Variable is exposed to lua but read only.
	*/
	ReadOnly,

	/**
	*	Variable is completely exposed to lua.
	*/
	ReadWrite
};