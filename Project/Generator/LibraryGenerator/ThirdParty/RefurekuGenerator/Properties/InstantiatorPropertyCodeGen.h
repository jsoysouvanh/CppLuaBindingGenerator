/**
*	Copyright (c) 2020 Julien SOYSOUVANH - All Rights Reserved
*
*	This file is part of the Refureku library project which is released under the MIT License.
*	See the LICENSE.md file for full license details.
*/

#pragma once

#include <Kodgen/CodeGen/Macro/MacroPropertyCodeGen.h>
#include <Kodgen/InfoStructures/MethodInfo.h>

namespace rfk
{
	class InstantiatorPropertyCodeGen : public kodgen::MacroPropertyCodeGen
	{
		protected:
			virtual bool generateClassFooterCodeForEntity(kodgen::EntityInfo const& entity, kodgen::Property const& /*property*/,
														  kodgen::uint8 /*propertyIndex*/, kodgen::MacroCodeGenEnv& env, std::string& inout_result) noexcept override
			{
				kodgen::MethodInfo const&	method		= static_cast<kodgen::MethodInfo const&>(entity);
				std::string					className	= method.outerEntity->getFullName();
				std::string					parameters	= method.getParameterTypes();
				std::string					methodPtr	= "&" + className + "::" + method.name;

				if (parameters.empty())
				{
					//SharedInstantiator with no parameters
					inout_result += "static_assert(std::is_invocable_r_v<rfk::SharedPtr<" + className + ">, decltype(" + methodPtr + ")>,"
						"\"[Refureku] Instantiator requires " + methodPtr + " to be a static method returning " + className + "* .\");" + env.getSeparator();
				}
				else
				{
					inout_result += "static_assert(std::is_invocable_r_v<rfk::SharedPtr<" + className + ">, decltype(" + methodPtr + "), " + std::move(parameters) + ">,"
						"\"[Refureku] Instantiator requires " + methodPtr + " to be a static method returning " + className + "*.\");" + env.getSeparator();
				}

				return true;
			}

		public:
			InstantiatorPropertyCodeGen() noexcept:
				kodgen::MacroPropertyCodeGen("rfk::Instantiator", kodgen::EEntityType::Method)
			{
			}

			/**
			*	@brief Generate code to add a custom instantiator to the generated class the method belongs to.
			* 
			*	@param method					The CustomInstantiator tagged method.
			*	@param generatedClassVarName	The name of the rfk::Struct / rfk::Class variable in the generated code
			*										that needs the custom instantiator, followed by -> or .
			*	@param generatedMethodVarName	Name of the rfk::StaticMethod variable in the generated code (the variable must be a pointer).
			*	@param inout_result				String to append the generated code.
			*/
			void addInstantiatorToClass(kodgen::Property const& /*property*/, std::string const& generatedClassVarName,
										std::string const& generatedMethodVarName, std::string& inout_result) const noexcept
			{
				inout_result += generatedClassVarName + "addSharedInstantiator(*" + generatedMethodVarName + "); ";
			}
	};
}