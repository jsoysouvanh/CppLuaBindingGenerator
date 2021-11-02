cmake_minimum_required(VERSION 3.13.5)

# Setup output directory
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/Lib)	# .lib / .a
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/Bin)	# .dll / .so / .dylib
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/Bin)	# .exe

# Build dependencies
add_subdirectory(ThirdParty/Kodgen)

set(CppLuaBindingGenerator CppLuaBindingGenerator)
add_executable(${CppLuaBindingGenerator} Src/main.cpp)

target_include_directories(${CppLuaBindingGenerator} PUBLIC Include)

target_link_libraries(${CppLuaBindingGenerator} PUBLIC Kodgen)

if (MSVC)

	# Setup warning level
	string(REGEX REPLACE " /W[0-4]" "" CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS}")	# Remove the default flag /W3
	target_compile_options(${CppLuaBindingGenerator} PRIVATE /W4 /MP)       # /MP for multithread compilation

else ()	# Clang or GCC

	target_compile_options(${CppLuaBindingGenerator} PRIVATE -Wall -Wextra -Wpedantic)

endif()

add_subdirectory(Tests)