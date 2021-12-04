//TODO: Copyright here

LuaFuncPropertyCodeGen::LuaFuncPropertyCodeGen() noexcept:
	kodgen::MacroPropertyCodeGen(funcLuaPropertyName.data(), kodgen::EEntityType::Method)
{
}

bool LuaFuncPropertyCodeGen::generateSourceFileHeaderCodeForEntity(kodgen::EntityInfo const& entity, kodgen::Property const& property,
																   kodgen::uint8 /*propertyIndex*/, kodgen::MacroCodeGenEnv& env, std::string& inout_result) noexcept
{
	//The CppLuaBindCodeGenModule already checked for the property validity, so we don't need to check again
	if (property.arguments.size() > 0u && property.arguments[0] == luaImplPropertyName)
	{
		std::string paramList;
		std::string paramName;

		//Run only for methods
		kodgen::MethodInfo const& method = static_cast<kodgen::MethodInfo const&>(entity);

		inout_result += method.returnType.getName() + " " + method.getFullName() + "(";

		if (!method.parameters.empty())
		{
			for (std::size_t i = 0; i < method.parameters.size(); i++)
			{
				paramName = "param" + std::to_string(i);

				inout_result += method.parameters[i].type.getName() + " " + paramName + ",";
				paramList += paramName + ",";
			}

			//Remove last ,
			inout_result.pop_back();
			paramList.pop_back();
		}
		
		inout_result += ")";

		if (method.isConst)
		{
			inout_result += " const";
		}

		//Start function definition
		inout_result += "{" + env.getSeparator();

		bool returnVoid = method.returnType.getCanonicalName() == "void";
		if (!returnVoid)
		{
			inout_result += "return ";
		}

		std::string luaFunctionName = (property.arguments.size() == 2u) ? property.arguments[1] : ("\"" + method.name + "\"");

		inout_result += "luaType[" + luaFunctionName + "].call";

		if (!returnVoid)
		{
			inout_result += "<" + method.returnType.getName() + ">";
		}

		inout_result += "(";

		//If the method is not static, must provide this as first param
		if (!method.isStatic)
		{
			inout_result += "this";

			if (!method.parameters.empty())
			{
				inout_result += ",";
			}
		}

		inout_result += paramList + ");" + env.getSeparator();

		//End function definition
		inout_result += "}" + env.getSeparator();
	}

	return true;
}

kodgen::int32 LuaFuncPropertyCodeGen::getGenerationOrder() const noexcept
{
	//Run after CppLuaBindCodeGenModule
	return 1;
}