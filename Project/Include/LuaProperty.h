//TODO: Copyright here

#pragma once

#include <string>
#include <cstdint>	//std::size_t

enum class ELuaProperty
{
	Undefined	= 0,

	/**
	*	Function is implemented in c++ and
	*	callable from both lua and c++.
	*/
	LuaExposed	= 1 << 0,

	/**
	*	Function is implemented in lua and
	*	callable from both lua and c++.
	*/
	LuaImpl		= 1 << 1,

	/**
	*	Variable is exposed to lua but read only.
	*/
	ReadOnly	= 1 << 2,

	/**
	*	Variable is completely exposed to lua.
	*/
	ReadWrite	= 1 << 3
};

ELuaProperty operator|(ELuaProperty lhs, ELuaProperty rhs)
{
	return static_cast<ELuaProperty>(static_cast<std::size_t>(lhs) | static_cast<std::size_t>(rhs));
}

bool overlap(ELuaProperty lhs, ELuaProperty rhs) noexcept
{
	return (static_cast<std::size_t>(lhs) | static_cast<std::size_t>(rhs)) != std::size_t(0u);
}

struct LuaProperty
{
	ELuaProperty	value;
	std::string		name;
};