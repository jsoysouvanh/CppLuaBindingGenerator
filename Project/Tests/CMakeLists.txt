cmake_minimum_required(VERSION 3.13.5)

project(CppLuaBindingGeneratorTests)

include(CTest)

add_subdirectory(ThirdParty/Lua)

###########################################
#		Configure the tests
###########################################

set(CppLuaBindingTests CppLuaBindingTests)
add_executable(${CppLuaBindingTests}
					main.cpp)

target_compile_features(${CppLuaBindingTests} PUBLIC cxx_std_17)

# Add include directories
target_include_directories(${CppLuaBindingTests}
							PUBLIC
								"Include"
								"ThirdParty/Lua/lua-5.4.3/include"
								"ThirdParty/sol2/include")

target_link_libraries(${CppLuaBindingTests} PRIVATE lua_static)

if (MSVC)
	target_compile_options(${CppLuaBindingTests} PRIVATE /MP)
endif()

# Create the command to run CppLuaBindingGenerator
set(RunTestGenerator RunTestGenerator)

add_custom_target(${RunTestGenerator}
					WORKING_DIRECTORY "${PROJECT_SOURCE_DIR}"
					COMMAND "CppLuaBindingGenerator" "${PROJECT_SOURCE_DIR}/TestsSettings.toml")

# Run the generator BEFORE building the project to refresh generated files
add_dependencies(${CppLuaBindingTests} ${RunTestGenerator})

add_test(NAME ${CppLuaBindingTests} COMMAND ${CppLuaBindingTests})