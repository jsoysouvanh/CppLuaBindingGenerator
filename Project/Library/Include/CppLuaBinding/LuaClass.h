/**
*	Copyright (c) 2021 Julien SOYSOUVANH - All Rights Reserved
*
*	This file is part of the Refureku library project which is released under the MIT License.
*	See the LICENSE.md file for full license details.
*/

#pragma once

#include <string>
#include <utility>	//std::move

namespace clb
{
	class LuaClass
	{
		private:
			//Name of the lua class binding. If empty, the class has the same name as the C++ class.
			std::string	_luaClassName;

		public:
			LuaClass() = default;

			/**
			*	@param luaClassName Name of the class binding.
			*/
			inline LuaClass(std::string luaClassName) noexcept:
				_luaClassName{ std::move(luaClassName) }
			{
			}

			/**
			*	@brief Getter for the field _luaClassName.
			* 
			*	@return _luaClassName.
			*/
			inline std::string const& getLuaClassName() const noexcept
			{
				return _luaClassName;
			}
	};
}