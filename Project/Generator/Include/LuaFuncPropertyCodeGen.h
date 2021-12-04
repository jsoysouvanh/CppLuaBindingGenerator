/**
*	Copyright (c) 2021 Julien SOYSOUVANH - All Rights Reserved
*
*	This file is part of the Refureku library project which is released under the MIT License.
*	See the LICENSE.md file for full license details.
*/

#pragma once

#include <string_view>

#include <Kodgen/CodeGen/Macro/MacroPropertyCodeGen.h>

namespace clb
{
	class LuaFuncPropertyCodeGen : public kodgen::MacroPropertyCodeGen
	{
		public:
			/**
			*	Name of the property used to bind a c++ method/function to lua.
			*	It takes 2 parameters (2nd is optional):
			*		- the 1st param defines if the method is implemented in c++ or in lua.
			*		- the 2nd param is the name of the method exposed to lua.
			*			If unspecified, defaults to the name of the C++ method.
			*/
			static constexpr std::string_view const funcLuaPropertyName		= "clb::LuaFunc";

			/**
			*	Make a c++ method implementable in lua instead of c++.
			*/
			static constexpr std::string_view const luaImplPropertyName		= "clb::LuaImpl";

			/**
			*	Make a c++ method exposed (callable from) to lua.
			*/
			static constexpr std::string_view const luaExposedPropertyName	= "clb::LuaExposed";

			LuaFuncPropertyCodeGen() noexcept;

			/**
			*	@brief Generate the implementation for lua functions with the LuaImpl qualifier.
			* 
			*	@param entity			Target method.
			*	@param property			property triggering the code generation.
			*	@param propertyIndex	Index of the property.
			*	@param env				Generation environment.
			*	@param inout_result		Generated code.
			* 
			*	@return true if the code generation succeeded, else false.
			*/
			virtual bool			generateSourceFileHeaderCodeForEntity(kodgen::EntityInfo const&	entity,
																		  kodgen::Property const&	property,
																		  kodgen::uint8				propertyIndex,
																		  kodgen::MacroCodeGenEnv&	env,
																		  std::string&				inout_result)	noexcept override;

			/**
			*	@brief	Run **after** the CppLuaBindCodeGenModule so that we don't need to check the property arguments validity again.
			* 
			*	@return 1.
			*/
			virtual kodgen::int32	getGenerationOrder()													const	noexcept override;
	};

	#include "LuaFuncPropertyCodeGen.inl"
}