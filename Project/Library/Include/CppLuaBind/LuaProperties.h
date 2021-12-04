/**
*	Copyright (c) 2021 Julien SOYSOUVANH - All Rights Reserved
*
*	This file is part of the CppLuaBind library project which is released under the MIT License.
*	See the LICENSE.md file for full license details.
*/

#pragma once

#include <string>
#include <utility>	//std::move

#if __has_include(<Refureku/Properties/PropertySettings.h>)
	//Include headers for Refureku compatibility
	#include <Refureku/Properties/PropertySettings.h> //rfk::PropertySettings, rfk::Property
	#include "CppLuaBind/Generated/LuaProperties_rfk_compat.h.h"
#else
	//Include headers without Refureku compatibility
	#include "CppLuaBind/Generated/LuaProperties_no_rfk_compat.h.h"
#endif

#include "CppLuaBind/Config.h"

namespace clb NAMESPACE()
{
	class CPPLUABIND_API
#ifdef REFUREKU_VERSION
		CLASS(rfk::PropertySettings(rfk::EEntityKind::Struct | rfk::EEntityKind::Class, false, false))
#else
		CLASS()
#endif
			LuaClass
#ifdef REFUREKU_VERSION
				: public rfk::Property
#endif
	{
		private:
			//Name of the lua class binding. If empty, the class has the same name as the C++ class.
			std::string	_bindingName;

		public:
			LuaClass() = default;

			/**
			*	@param luaClassName Name of the class binding.
			*/
			inline LuaClass(std::string bindingName) noexcept:
				_bindingName{ std::move(bindingName) }
			{
			}

			/**
			*	@brief Getter for the field _luaClassName.
			* 
			*	@return _luaClassName.
			*/
			inline std::string const& getBindingName() const noexcept
			{
				return _bindingName;
			}

		clb_LuaClass_GENERATED
	};

	//Use unscoped enum on purpose to avoid boilerplate when declaring variable properties.
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

	class CPPLUABIND_API
#ifdef REFUREKU_VERSION
		CLASS(rfk::PropertySettings(rfk::EEntityKind::Field, false))
#else
		CLASS()
#endif
			LuaVar
#ifdef REFUREKU_VERSION
				: public rfk::Property
#endif
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

		clb_LuaVar_GENERATED
	};

	//Use unscoped enum on purpose to avoid boilerplate when declaring function properties.
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

	class CPPLUABIND_API
#ifdef REFUREKU_VERSION
		CLASS(rfk::PropertySettings(rfk::EEntityKind::Method, false))
#else
		CLASS()
#endif
			LuaFunc
#ifdef REFUREKU_VERSION
				: public rfk::Property
#endif
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

		clb_LuaFunc_GENERATED
	};
}

File_LuaProperties_GENERATED