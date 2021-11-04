//TODO: Copyright here

CppLuaBindCodeGenModule::CppLuaBindCodeGenModule() noexcept
{
	addPropertyCodeGen(_luaFuncPropertyCodeGen);
}

CppLuaBindCodeGenModule::CppLuaBindCodeGenModule(CppLuaBindCodeGenModule const&) noexcept:
	CppLuaBindCodeGenModule()
{
}

CppLuaBindCodeGenModule* CppLuaBindCodeGenModule::clone() const noexcept
{
	return new CppLuaBindCodeGenModule(*this);
}

bool CppLuaBindCodeGenModule::initialGenerateHeaderFileHeaderCode(kodgen::MacroCodeGenEnv& env, std::string& inout_result) noexcept
{
	inout_result += "#include <sol/sol.hpp>" + env.getSeparator();

	return true;
}

kodgen::ETraversalBehaviour CppLuaBindCodeGenModule::generateClassFooterCodeForEntity(kodgen::EntityInfo const& entity,
																					  kodgen::MacroCodeGenEnv& env, std::string& inout_result) noexcept
{
	//The entity is a struct or a class
	if (entity.entityType && (kodgen::EEntityType::Class | kodgen::EEntityType::Struct))
	{
		kodgen::StructClassInfo const& class_ = static_cast<kodgen::StructClassInfo const&>(entity);

		generateClassFooterCodeForClass(class_, env, inout_result);

		//Iterate on nested entities if it has at least 1 nested struct or class that could require lua bindings
		//Otherwise, just move on to the next class in the AST
		return (!class_.nestedClasses.empty() || !class_.nestedStructs.empty()) ?
					kodgen::ETraversalBehaviour::Recurse :
					kodgen::ETraversalBehaviour::Continue;
	}
	else
	{
		//Only namespaces can contain structs/classes. Ignore all other entities.
		//TODO: Should support variables and functions bindings later
		return (entity.entityType == kodgen::EEntityType::Namespace) ?
					kodgen::ETraversalBehaviour::Recurse :
					kodgen::ETraversalBehaviour::Break;
	}
}

kodgen::ETraversalBehaviour CppLuaBindCodeGenModule::generateSourceFileHeaderCodeForEntity(kodgen::EntityInfo const& entity,
																						   kodgen::MacroCodeGenEnv& env, std::string& inout_result) noexcept
{
	//The entity is a struct or a class
	if (entity.entityType && (kodgen::EEntityType::Class | kodgen::EEntityType::Struct))
	{
		kodgen::StructClassInfo const& class_ = static_cast<kodgen::StructClassInfo const&>(entity);

		generateSourceCodeForClass(class_, env, inout_result);

		//Iterate on nested entities if it has at least 1 nested struct or class that could require lua bindings
		//Otherwise, just move on to the next class in the AST
		return (!class_.nestedClasses.empty() || !class_.nestedStructs.empty()) ?
			kodgen::ETraversalBehaviour::Recurse :
			kodgen::ETraversalBehaviour::Continue;
	}
	else
	{
		//Only namespaces can contain structs/classes. Ignore all other entities.
		//TODO: Should support variables and functions bindings later
		return (entity.entityType == kodgen::EEntityType::Namespace) ?
			kodgen::ETraversalBehaviour::Recurse :
			kodgen::ETraversalBehaviour::Break;
	}
}

bool CppLuaBindCodeGenModule::generateClassFooterCodeForClass(kodgen::StructClassInfo const& class_,
															  kodgen::MacroCodeGenEnv& env, std::string& inout_result) noexcept
{
	std::string error;

	//Check class eligibility to lua binding
	if (auto prop = getLuaProperty(class_, error))
	{
		env.getLogger()->log("Valid class: " + class_.name);
		_validEntities.insert({ &class_, *prop });

		//Generate the sol::userType field
		inout_result += "private: static sol::usertype<" + class_.name + "> luaType;" + env.getSeparator();

		inout_result += "public:" + env.getSeparator() +
			"static bool initLuaBinding(sol::state& luaState);" + env.getSeparator() +
			"static bool deinitLuaBinding();" + env.getSeparator();
	}
	
	if (!error.empty())
	{
		if (env.getLogger() != nullptr)
		{
			env.getLogger()->log(error, kodgen::ILogger::ELogSeverity::Error);
		}

		return false;
	}

	return true;
}

bool CppLuaBindCodeGenModule::generateSourceCodeForClass(kodgen::StructClassInfo const& class_,
														 kodgen::MacroCodeGenEnv& env, std::string& inout_result) noexcept
{
	if (_validEntities.find(&class_) != _validEntities.cend())
	{
		//Define sol::userType field
		std::string classFullName = class_.getFullName();
		inout_result += "sol::usertype<" + classFullName + "> " + classFullName + "::luaType = sol::lua_nil;" + env.getSeparator() + env.getSeparator();

		//Define fields
		return generateInitLuaBindingDefinition(class_, env, inout_result) &&
				generateDeinitLuaBindingDefinition(class_, env, inout_result);
	}

	return true;
}

bool CppLuaBindCodeGenModule::generateInitLuaBindingDefinition(kodgen::StructClassInfo const& class_,
															   kodgen::MacroCodeGenEnv& env, std::string& inout_result) noexcept
{
	auto isConstFieldLambda = [](kodgen::FieldInfo const& field) -> bool
	{
		return (field.type.typeParts.front().descriptor & kodgen::ETypeDescriptor::Const) != kodgen::ETypeDescriptor::Undefined;
	};

	std::string error;

	auto it = _validEntities.find(&class_);

	inout_result += "bool " + class_.getFullName() + "::initLuaBinding(sol::state& luaState) {" + env.getSeparator() +
		"if (luaType.lua_state() == nullptr)" + env.getSeparator() +
		"{" + env.getSeparator() +
			"luaType = luaState.new_usertype<" + class_.name + ">(" + _validEntities.find(&class_)->second.name + ");" + env.getSeparator();

	//Generate fields
	for (kodgen::FieldInfo const& field : class_.fields)
	{
		if (auto prop = getLuaProperty(field, error))
		{
			if (prop->value == ELuaProperty::ReadOnly || isConstFieldLambda(field))
			{
				inout_result += "luaType.set(" + prop->name + ", sol::readonly(&" + field.getFullName() + "));" + env.getSeparator();
			}
			else
			{
				inout_result += "luaType.set(" + prop->name + ", &" + field.getFullName() + ");" + env.getSeparator();
			}
		}
		else if (!error.empty())
		{
			if (env.getLogger() != nullptr)
			{
				env.getLogger()->log(error, kodgen::ILogger::ELogSeverity::Error);
			}

			return false;
		}
	}

	//Generate methods / static methods
	for (kodgen::MethodInfo const& method : class_.methods)
	{
		if (auto prop = getLuaProperty(method, error))
		{
			if (prop->value == ELuaProperty::LuaExposed)
			{
				inout_result += "luaType.set(" + prop->name + ", &" + method.getFullName() + ");" + env.getSeparator();
			}
		}
		else if (!error.empty())
		{
			if (env.getLogger() != nullptr)
			{
				env.getLogger()->log(error, kodgen::ILogger::ELogSeverity::Error);
			}

			return false;
		}
	}

	inout_result += "return true;" + env.getSeparator() +
					"}" + env.getSeparator();

	inout_result += "return false; }" + env.getSeparator() + env.getSeparator();

	return true;
}

bool CppLuaBindCodeGenModule::generateDeinitLuaBindingDefinition(kodgen::StructClassInfo const& class_,
																 kodgen::MacroCodeGenEnv& env, std::string& inout_result) noexcept
{
	inout_result += "bool " + class_.getFullName() + "::deinitLuaBinding() {" + env.getSeparator() +
		"if (luaType.lua_state() != nullptr) {" + env.getSeparator() +
			"luaType.unregister();" + env.getSeparator() +
			"luaType = sol::lua_nil;" + env.getSeparator() +
			"return true;" + env.getSeparator() +
		"}" + env.getSeparator() +
		"return false;" + env.getSeparator() +
	"}" + env.getSeparator();

	return true;
}

std::optional<LuaProperty> CppLuaBindCodeGenModule::getLuaProperty(kodgen::EntityInfo const& entity, std::string& errorMessage) noexcept
{
	auto getClassLuaPropertyLambda = [](kodgen::EntityInfo const& entity, std::string& errorMessage) -> std::optional<LuaProperty>
	{
		kodgen::Property const* prop = findProperty(entity, classLuaPropertyName);

		if (prop != nullptr)
		{
			//Must have at most 1 argument
			if (prop->arguments.size() > 1u)
			{
				errorMessage = std::string(classLuaPropertyName) + " must have at most 1 argument (1: name of the lua class).";
			}
			else if (prop->arguments.size() == 1u)
			{
				if (!isValidLuaName(prop->arguments[0]))
				{
					errorMessage = std::string(classLuaPropertyName) + " first argument must be a string designing the lua class name.";
				}
				else
				{
					return LuaProperty{ ELuaProperty::Undefined, prop->arguments[0] };
				}
			}
			else //no args
			{
				return LuaProperty{ ELuaProperty::Undefined, quoteString(entity.name) };
			}
		}

		return opt::nullopt;
	};
	
	auto getVarLuaPropertyLambda = [](kodgen::EntityInfo const& entity, std::string& errorMessage) -> std::optional<LuaProperty>
	{
		kodgen::Property const* prop = findProperty(entity, varLuaPropertyName);

		if (prop != nullptr)
		{
			//Must have 1 or 2 args (2nd arg is optional)
			if (prop->arguments.size() != 1u && prop->arguments.size() != 2u)
			{
				errorMessage = std::string(varLuaPropertyName) + " must have 1 or 2 arguments (1: " + std::string(readonlyPropertyName) +
								" or " + std::string(readWritePropertyName) + ", 2: name of the lua variable).";
			}
			else
			{
				//Check 1st arg
				auto it = luaPropertyNameValue.find(prop->arguments[0]);

				if (it == luaPropertyNameValue.cend() || !overlap(it->second, ELuaProperty::ReadOnly | ELuaProperty::ReadWrite))
				{
					errorMessage = std::string(varLuaPropertyName) + " 1st argument must be one of " + std::string(readonlyPropertyName) +
									" or " + std::string(readWritePropertyName) + ".";
					return opt::nullopt;
				}

				ELuaProperty luaPropertyValue = it->second;

				//Check 2nd arg
				std::string luaVarName;
				if (prop->arguments.size() == 2u)
				{
					if (!isValidLuaName(prop->arguments[1]))
					{
						errorMessage = std::string(varLuaPropertyName) + " 2nd argument must be a string designing the lua variable name.";
						return opt::nullopt;
					}
					else
					{
						luaVarName = prop->arguments[1];
					}
				}
				else //No 2nd argument, defaults to the entity name
				{
					luaVarName = quoteString(entity.name);
				}

				return LuaProperty{ luaPropertyValue, std::move(luaVarName) };
			}
		}

		return opt::nullopt;
	};

	auto getFuncLuaPropertyLambda = [](kodgen::EntityInfo const& entity, std::string& errorMessage) -> std::optional<LuaProperty>
	{
		kodgen::Property const* prop = findProperty(entity, LuaFuncPropertyCodeGen::funcLuaPropertyName);

		if (prop != nullptr)
		{
			//Must have 1 or 2 args (2nd arg is optional)
			if (prop->arguments.size() != 1u && prop->arguments.size() != 2u)
			{
				errorMessage = std::string(LuaFuncPropertyCodeGen::funcLuaPropertyName) + " must have 1 or 2 arguments (1: " + std::string(LuaFuncPropertyCodeGen::luaImplPropertyName) +
					" or " + std::string(LuaFuncPropertyCodeGen::luaExposedPropertyName) + ", 2: name of the lua function).";
			}
			else
			{
				//Check 1st arg
				auto it = luaPropertyNameValue.find(prop->arguments[0]);

				if (it == luaPropertyNameValue.cend() || !overlap(it->second, ELuaProperty::LuaImpl | ELuaProperty::LuaExposed))
				{
					errorMessage = std::string(LuaFuncPropertyCodeGen::funcLuaPropertyName) + " 1st argument must be one of " + std::string(LuaFuncPropertyCodeGen::luaImplPropertyName) +
						" or " + std::string(LuaFuncPropertyCodeGen::luaExposedPropertyName) + ".";
					return opt::nullopt;
				}

				ELuaProperty luaPropertyValue = it->second;

				//Check 2nd arg
				std::string luaFuncName;
				if (prop->arguments.size() == 2u)
				{
					if (!isValidLuaName(prop->arguments[1]))
					{
						errorMessage = std::string(LuaFuncPropertyCodeGen::funcLuaPropertyName) + " 2nd argument must be a string designing the lua function name.";
						return opt::nullopt;
					}
					else
					{
						luaFuncName = prop->arguments[1];
					}
				}
				else //No 2nd argument, defaults to the entity name
				{
					luaFuncName = quoteString(entity.name);
				}

				return LuaProperty{ luaPropertyValue, std::move(luaFuncName) };
			}
		}

		return opt::nullopt;
	};

	switch (entity.entityType)
	{
		case kodgen::EEntityType::Struct:
			[[fallthrough]];
		case kodgen::EEntityType::Class:
			return getClassLuaPropertyLambda(entity, errorMessage);

		case kodgen::EEntityType::Field:
			//TODO: might add kodgen::EEntityType::Variable in the same case later
			return getVarLuaPropertyLambda(entity, errorMessage);

		case kodgen::EEntityType::Method:
			//TODO: might add kodgen::EEntityType::Function in the same case later
			return getFuncLuaPropertyLambda(entity, errorMessage);

		default:
			break;
	}

	return opt::nullopt;
}

kodgen::Property const* CppLuaBindCodeGenModule::findProperty(kodgen::EntityInfo const& entity, std::string_view const& propertyName) noexcept
{
	for (kodgen::Property const& prop : entity.properties)
	{
		if (prop.name == propertyName)
		{
			return &prop;
		}
	}

	return nullptr;
}

bool CppLuaBindCodeGenModule::isValidLuaName(std::string const& name) noexcept
{
	return name.size() >= 2u && name.front() == '"' && name.back() == '"';
}

std::string CppLuaBindCodeGenModule::quoteString(std::string string) noexcept
{
	return "\"" + std::move(string) + "\"";
}