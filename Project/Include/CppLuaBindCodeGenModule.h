//TODO: Copyright here

#pragma once

#include <string>
#include <string_view>
#include <unordered_map>
#include <optional>

#include <Kodgen/CodeGen/Macro/MacroCodeGenModule.h>

#include "LuaProperty.h"

class CppLuaBindCodeGenModule : public kodgen::MacroCodeGenModule
{
	private:
		/**
		*	Collection of entities used to generate code.
		*	This collection is used to avoid checking if an entity is eligible for code generation multiple times.
		*/
		std::unordered_map<kodgen::EntityInfo const*, LuaProperty>	_validEntities;

		/**
		*	@brief	Retrieve the lua property from an entity. If no lua property is found, return an empty optional.
		*			If there is a syntax error, errorMessage is filled.
		* 
		*	@param entity		Entity we retrieve properties from.
		*	@param errorMessage	String to fill in case a syntax error is detected.
		* 
		*	@return A filled optional if a valid lua property was found, else an empty optional.
		*/
		static std::optional<LuaProperty>	getLuaProperty(kodgen::EntityInfo const&	entity,
														   std::string&					errorMessage)	noexcept;

		/**
		*	@brief Quote the provided string by adding " char at the beginning and the end.
		* 
		*	@param string The string to quote.
		* 
		*	@return The quoted string.
		*/
		static std::string					quoteString(std::string string)								noexcept;

		/**
		*	@brief Find a property named propertyName in the provided entity.
		* 
		*	@param entity		Target entity.
		*	@param propertyName	Name of the property we are looking for.
		* 
		*	@return A pointer to the first property named propertyNamed in the entity, or nullptr if none could be found.
		*/
		static kodgen::Property const*		findProperty(kodgen::EntityInfo const&	entity,
														 std::string_view const&	propertyName)		noexcept;

		/**
		*	@brief Check whether a provided name is valid or not (starts with " and end with ").
		* 
		*	@param name The name to check.
		* 
		*	@return true if the provided luaName is valid, else false.
		*/
		static bool							isValidLuaName(std::string const& name)						noexcept;

		/**
		*	@brief Generate initLuaBinding and deinitLuaBinding methods.
		* 
		*	@param class_		Target class.
		*	@param env			Generation environment.
		*	@param inout_result	Generated code.
		* 
		*	@return true if the generation completed without errors, else false.
		*/
		bool generateClassFooterCodeForClass(kodgen::StructClassInfo const& class_,
											 kodgen::MacroCodeGenEnv&		env,
											 std::string&					inout_result)	noexcept;

		bool generateSourceCodeForClass(kodgen::StructClassInfo const& class_,
										kodgen::MacroCodeGenEnv&		env,
										std::string&					inout_result)	noexcept;

	public:
		/**
		*	Name of the property used to bind a c++ class to lua.
		*	It takes the name of the bound lua class as optional parameter.
		*	If unspecified, defaults to the name of the C++ class.
		*/
		static constexpr std::string_view const classLuaPropertyName	= "LuaClass";

		/**
		*	Name of the property used to bind a c++ variable/field to lua.
		*	It takes 2 parameters (2nd is optional):
		*		- the 1st param defines if the field is readwrite or readonly from the lua side.
		*			If the field is const, automatically falls back to readonly.
		*		- the 2nd param is the name of the field exposed to lua.
		*			If unspecified, defaults to the name of the C++ field.
		*/
		static constexpr std::string_view const varLuaPropertyName		= "LuaVar";

		/**
		*	Defines a lua field as readonly.
		*/
		static constexpr std::string_view const readonlyPropertyName	= "ReadOnly";

		/**
		*	Defines a lua field as readwrite.
		*/
		static constexpr std::string_view const readWritePropertyName	= "ReadWrite";

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

		/**
		*	Association between a property name and its matching ELuaProperty value.
		*/
		static inline std::unordered_map<std::string_view, ELuaProperty> luaPropertyNameValue
		{
			{ readonlyPropertyName, ELuaProperty::ReadOnly },
			{ readWritePropertyName, ELuaProperty::ReadWrite },
			{ luaImplPropertyName, ELuaProperty::LuaImpl },
			{ luaExposedPropertyName, ELuaProperty::LuaExposed },
		};

		virtual CppLuaBindCodeGenModule*	clone()																	const	noexcept	override;

		/**
		*	@brief Generate code to include sol header.
		*/
		virtual bool						initialGenerateHeaderFileHeaderCode(kodgen::MacroCodeGenEnv&	env,
																				std::string&				inout_result)	noexcept	override;

		/**
		*	@brief Generate initLuaBinding and deinitLuaBinding methods if entity is a class with the LuaClass property.
		*/
		virtual kodgen::ETraversalBehaviour	generateClassFooterCodeForEntity(kodgen::EntityInfo const&	entity,
																			 kodgen::MacroCodeGenEnv&	env,
																			 std::string&				inout_result)		noexcept	override;
};

#include "CppLuaBindCodeGenModule.inl"