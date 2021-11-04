//TODO: Copyright here

#pragma once

#include <string_view>

#include <Kodgen/CodeGen/Macro/MacroPropertyCodeGen.h>

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
		static constexpr std::string_view const funcLuaPropertyName		= "LuaFunc";

		/**
		*	Make a c++ method implementable in lua instead of c++.
		*/
		static constexpr std::string_view const luaImplPropertyName		= "LuaImpl";

		/**
		*	Make a c++ method exposed (callable from) to lua.
		*/
		static constexpr std::string_view const luaExposedPropertyName	= "LuaExposed";

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