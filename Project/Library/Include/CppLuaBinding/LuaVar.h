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
	//Use unscoped enum on purpose to avoid boilerplate when declaring variables.
	enum ELuaVarBindingPolicy
	{
		/**
		*	Variable is exposed to lua but read only.
		*/
		ReadOnly,

		/**
		*	Variable is completely exposed to lua.
		*/
		ReadWrite
	};

	class LuaVar
	{
		private:
			//Binding policy for this variable.
			ELuaVarBindingPolicy	_bindingPolicy;

			//Name of the lua variable binding.
			std::string				_bindingName;

		public:
			inline LuaVar(ELuaVarBindingPolicy	bindingPolicy = ReadWrite,
						  std::string			bindingName   = "") noexcept:
				_bindingPolicy{bindingPolicy},
				_bindingName{std::move(bindingName)}
			{
			}

			/**
			*	@brief Getter for the field _bindingPolicy.
			* 
			*	@return _bindingPolicy.
			*/
			inline ELuaVarBindingPolicy getBindingPolicy() const noexcept
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