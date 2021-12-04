project(CppLuaBindLibrary)

# Add Refureku library
set(CppLuaBindLibraryTarget CppLuaBind)
add_library(${CppLuaBindLibraryTarget}
				SHARED
					"Src/LuaProperties.cpp")

# Setup language requirements
target_compile_features(${CppLuaBindLibraryTarget} PUBLIC cxx_std_17)

target_compile_definitions(${CppLuaBindLibraryTarget} PRIVATE CPPLUABIND_EXPORT)

target_include_directories(${CppLuaBindLibraryTarget} PUBLIC Include)

#TODO: Add Refureku include dir with specific condition
#target_include_directories(${CppLuaBindLibraryTarget} PRIVATE "ThirdParty")

# Setup compilation flags
if (MSVC)

	target_compile_options(${CppLuaBindLibraryTarget} PRIVATE /MP)				# Multithread compilation

	string(REGEX REPLACE "/W[0-4]" "/W4" CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS}")	# Replace the default flag /W3 by /W4

	if (CLB_DISABLE_RTTI)
		string(REGEX REPLACE "/GR" "/GR-" CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS}")	# Replace the default flag /GR by /GR- to disable RTTI
	endif()

else()	# Clang or GCC

	target_compile_options(${CppLuaBindLibraryTarget} PRIVATE -Wall -Wextra -Wpedantic -fvisibility-ms-compat -fvisibility-inlines-hidden)

	if (CLB_DISABLE_RTTI)
		target_compile_options(${CppLuaBindLibraryTarget} PRIVATE -fno-rtti)
	endif()

endif()