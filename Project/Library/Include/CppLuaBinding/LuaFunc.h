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
	//Use unscoped enum on purpose to avoid boilerplate when declaring functions.
	enum ELuaFuncBindingPolicy
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
		LuaImpl
	};

	class LuaFunc
	{
		private:
			//Binding policy for this function.
			ELuaFuncBindingPolicy	_bindingPolicy;

			//Name of the lua function binding.
			std::string				_bindingName;

		public:
			inline LuaFunc(ELuaFuncBindingPolicy bindingPolicy = LuaExposed,
						   std::string			 bindingName   = "") noexcept:
				_bindingPolicy{bindingPolicy},
				_bindingName{std::move(bindingName)}
			{
			}

			/**
			*	@brief Getter for the field _bindingPolicy.
			* 
			*	@return _bindingPolicy.
			*/
			inline ELuaFuncBindingPolicy getBindingPolicy() const noexcept
			{
				return _bindingPolicy;
			}

			/**
			*	@brief Getter for the field _bindingName.
			* 
			*	@return _bindingName.
			*/
			inline std::string const& getBindingName() const noexcept
			{
				return _bindingName;
			}
	};
}