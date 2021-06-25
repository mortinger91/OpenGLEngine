# Version
cmake_minimum_required(VERSION 3.17 FATAL_ERROR)
# Policies
cmake_policy(SET CMP0077 NEW) # option honors normal variables
cmake_policy(SET CMP0072 NEW)

if(NOT WIN32)
  string(ASCII 27 Esc)
  set(ColourReset "${Esc}[m")
  set(ColourBold  "${Esc}[1m")
  set(Red         "${Esc}[31m")
  set(Green       "${Esc}[32m")
  set(Yellow      "${Esc}[33m")
  set(Blue        "${Esc}[34m")
  set(Magenta     "${Esc}[35m")
  set(Cyan        "${Esc}[36m")
  set(White       "${Esc}[37m")
  set(BoldRed     "${Esc}[1;31m")
  set(BoldGreen   "${Esc}[1;32m")
  set(BoldYellow  "${Esc}[1;33m")
  set(BoldBlue    "${Esc}[1;34m")
  set(BoldMagenta "${Esc}[1;35m")
  set(BoldCyan    "${Esc}[1;36m")
  set(BoldWhite   "${Esc}[1;37m")
endif()

# Name
set(PROJECT_NAME "OpenGLEngine")
set(PROJECT_DESCRIPTION "Another OpenGL 3D Rendering Engine")
set(AUTHOR "mortinger91")
set(AUTHOR_DOMAIN "https://github.com/${AUTHOR}/${PROJECT_NAME}")

# Version
set(VERSION_MAJOR "1")
set(VERSION_MINOR "0")
set(VERSION_PATCH "0")
set(VERSION_TWEAK "0")
set(VERSION "${VERSION_MAJOR}.${VERSION_MINOR}.${VERSION_PATCH}.${VERSION_TWEAK}")

# Targets
set(LIBRARY_TARGET_NAME "${PROJECT_NAME}")

option(BUILD_SHARED_LIBS "Build shared libraries" OFF)

# Configs
list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_LIST_DIR}/cmake")
set_property(GLOBAL PROPERTY USE_FOLDERS ON)

# Project
project(${PROJECT_NAME}
		VERSION ${VERSION}
		DESCRIPTION ${PROJECT_DESCRIPTION}
		HOMEPAGE_URL ${AUTHOR_DOMAIN})

### Language Options
# C
set(C_STANDARD_DEFAULT 11)
set(CMAKE_C_STANDARD ${C_STANDARD_DEFAULT})
set(CMAKE_C_STANDARD_REQUIRED ON)
set(CMAKE_C_EXTENSIONS OFF)
# C++
set(CXX_STANDARD_DEFAULT 17)
set(CMAKE_CXX_STANDARD ${CXX_STANDARD_DEFAULT})
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

### Modules
#include(WriteCompilerDetectionHeader)
#include(GenerateExportHeader)

add_subdirectory(vendor)

find_package(OpenGL REQUIRED)

set(BUILD_FOLDER ${CMAKE_CURRENT_BINARY_DIR})
add_subdirectory(engine)

target_link_libraries(
		${LIBRARY_TARGET_NAME}
		PUBLIC
		imgui
		glfw
		libglew_static
		OpenGL::GL
		glm::glm
)

add_executable(
			exec engine/src/Main.cpp
)

	target_link_libraries(
			exec
			PRIVATE
			${LIBRARY_TARGET_NAME}
)

include(build)