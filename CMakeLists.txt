cmake_minimum_required(VERSION 3.20)

project(opengl-tests LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Use packages provided by vcpkg (configure CMake with the vcpkg toolchain)
find_package(glew REQUIRED)
find_package(glfw3 CONFIG REQUIRED)
find_package(OpenGL REQUIRED)
find_package(glm REQUIRED)

add_executable(${PROJECT_NAME} src/main.cpp "src/utils/fs.cpp" "src/utils/fs.cpp" "src/utils/progman.cpp")

file(COPY ${CMAKE_SOURCE_DIR}/src/shaders DESTINATION ${CMAKE_BINARY_DIR})
if (TARGET glfw3::glfw)
	target_link_libraries(${PROJECT_NAME} PRIVATE GLEW::GLEW glfw3::glfw OpenGL::GL glm::glm)
elseif (TARGET glfw)
	# Some vcpkg ports export the GLFW target as 'glfw'
	target_link_libraries(${PROJECT_NAME} PRIVATE GLEW::GLEW glfw OpenGL::GL glm::glm)
else()
	message(WARNING "Imported target glfw3::glfw or glfw not found. Falling back to locating library file in VCPKG installation.")
	if(DEFINED ENV{VCPKG_ROOT} AND DEFINED VCPKG_TARGET_TRIPLET)
		set(_vcpkg_lib_dir "$ENV{VCPKG_ROOT}/installed/${VCPKG_TARGET_TRIPLET}/lib")
		set(_vcpkg_debug_lib_dir "$ENV{VCPKG_ROOT}/installed/${VCPKG_TARGET_TRIPLET}/debug/lib")
	else()
		set(_vcpkg_lib_dir "${CMAKE_SOURCE_DIR}/vcpkg_installed/lib")
		set(_vcpkg_debug_lib_dir "${CMAKE_SOURCE_DIR}/vcpkg_installed/debug/lib")
	endif()

	find_library(GLFW3_LIB NAMES glfw3 glfw PATHS ${_vcpkg_lib_dir} ${_vcpkg_debug_lib_dir} NO_DEFAULT_PATH)
	if(GLFW3_LIB)
		message(STATUS "Found glfw library: ${GLFW3_LIB}")
		target_link_libraries(${PROJECT_NAME} PRIVATE GLEW::GLEW ${GLFW3_LIB} OpenGL::GL glm::glm)
	else()
		message(FATAL_ERROR "Could not locate glfw library. Make sure glfw3 is installed via vcpkg for the active triplet and CMAKE_TOOLCHAIN_FILE is set to vcpkg.cmake. Also verify VCPKG_TARGET_TRIPLET (for example: x64-windows) matches your build.")
	endif()
endif()
