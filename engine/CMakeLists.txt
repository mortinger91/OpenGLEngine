message(STATUS "   ${Red}Configuring ${PROJECT_NAME} ${VERSION}${ColourReset}")

# Target
add_library(
		${LIBRARY_TARGET_NAME}
		STATIC

		src/Camera.h
		src/IndexBuffer.h
		src/Material.h
		src/Mesh.h
		src/MeshBuilder.h
		src/PointLight.h
		src/Renderer.h
		src/Shader.h
		src/Texture.h
		src/Utility.h
		src/VertexArray.h
		src/VertexBuffer.h
		src/VertexBufferLayout.h
		src/libs/stb_image/stb_image.h
		src/tests/Test.h
		src/tests/Test3DCube.h
		src/tests/TestClearColor.h

		src/Camera.cpp
		src/IndexBuffer.cpp
		src/Material.cpp
		src/Mesh.cpp
		src/Renderer.cpp
		src/Shader.cpp
		src/Texture.cpp
		src/VertexArray.cpp
		src/VertexBuffer.cpp
		src/libs/stb_image/stb_image.cpp
		src/tests/Test.cpp
		src/tests/Test3DCube.cpp
		src/tests/TestClearColor.cpp
)

# target_precompile_headers(
		# ${LIBRARY_TARGET_NAME}
		# PRIVATE
		#"include/real/pch.hpp"
# )

set (source "${CMAKE_SOURCE_DIR}/engine/res")
if (UNIX)
	set (destination "${BUILD_FOLDER}/bin/res")
	add_custom_command(
	 TARGET ${LIBRARY_TARGET_NAME} PRE_BUILD
	 COMMAND ${CMAKE_COMMAND} -E create_symlink ${source} ${destination} 
	 DEPENDS ${destination}
	 COMMENT "symbolic link resources folder from ${source} => ${destination}"
	)
	message(STATUS "   ${Red}symbolic link resources folder from ${source} => ${destination}${ColourReset}")
endif (UNIX)
if (WIN32)
	# TODO: make it automatic depending on the build configuration selected
	set (destination "${BUILD_FOLDER}/bin")
	#add_custom_command(
	# TARGET ${LIBRARY_TARGET_NAME} PRE_BUILD
	# COMMAND ${CMAKE_COMMAND} -E copy_directory ${source} ${destination} 
	# DEPENDS ${destination}
	# COMMENT "copied resources folder from ${source} => ${destination}"
	#)
	file(COPY ${source} DESTINATION ${destination})
	message(STATUS "   ${Red}copied resources folder from ${source} => ${destination}${ColourReset}")
endif (WIN32)

# Properties
target_include_directories(
		${LIBRARY_TARGET_NAME}
		PUBLIC
		"src"
		"src/libs"
		"res"
		"../vendor/glew-cmake/include"
		"../vendor/glfw/include"
		"../vendor/glm/glm"
		"../vendor/imgui-cmake"
		#"${CMAKE_CURRENT_BINARY_DIR}"
		#PRIVATE
		#"src"
)

set_target_properties(
		${LIBRARY_TARGET_NAME} PROPERTIES

		# Folder
		FOLDER ${PROJECT_NAME}/engine

		# Version
		VERSION ${VERSION}
		SOVERSION ${VERSION_MAJOR}

		# C++
		CXX_STANDARD ${CXX_STANDARD_DEFAULT}
		CXX_STANDARD_REQUIRED ON
		CXX_EXTENSIONS OFF

		# C
		C_STANDARD ${C_STANDARD_DEFAULT}
		C_STANDARD_REQUIRED ON
		C_EXTENSIONS OFF

		# Other
		VS_DEBUGGER_WORKING_DIRECTORY "${CMAKE_RUNTIME_OUTPUT_DIRECTORY}"
)

target_compile_definitions(
		${LIBRARY_TARGET_NAME}
		PRIVATE

		PUBLIC
		# Build type macros
		$<$<CONFIG:Debug>:DEBUG>
		$<$<CONFIG:Release>:RELEASE>
		$<$<CONFIG:Profile>:PROFILE>
		# Platform macros
		$<$<PLATFORM_ID:Windows>:PLATFORM_WINDOWS>
		INTERFACE
		CLIENT
)

target_compile_options(
		${LIBRARY_TARGET_NAME}
		PUBLIC
		${DEFAULT_COMPILER_OPTIONS}
)

target_link_options(
		${LIBRARY_TARGET_NAME}
		PRIVATE
		${DEFAULT_LINKER_OPTIONS}
)

set_target_properties(
		${LIBRARY_TARGET_NAME} PROPERTIES

		VISIBILITY_INLINES_HIDDEN YES
		CXX_VISIBILITY_PRESET hidden
		C_VISIBILITY_PRESET hidden
		ASM_VISIBILITY_PRESET hidden
)

# Utility
# Generated Files
write_compiler_detection_header(
		FILE "${PROJECT_NAME}/core-compiler.hpp"
		PREFIX ${PROJECT_NAME}
		COMPILERS Clang GNU MSVC
		FEATURES cxx_override
)

generate_export_header(
		${LIBRARY_TARGET_NAME}
		BASE_NAME "${PROJECT_NAME}"
		PREFIX_NAME "${PROJECT_NAME}_"
		INCLUDE_GUARD_NAME "${PROJECT_NAME}_CORE_COMPILER"
		EXPORT_MACRO_NAME "API"
		EXPORT_FILE_NAME "${PROJECT_NAME}/core-export.hpp"
		NO_EXPORT_MACRO_NAME "NO_EXPORT"
		DEPRECATED_MACRO_NAME "DEPRECATED"
		INCLUDE_GUARD_NAME "${PROJECT_NAME}_CORE_API"
		STATIC_DEFINE "STATIC"
		NO_DEPRECATED_MACRO_NAME "NO_DEPRECATED"

		# DEFINE_NO_DEPRECATED
)

# Custom commands
set(LOGS_DIR "${CMAKE_CURRENT_BINARY_DIR}/logs")
set(LOG_EXPORTS_FILE "${LOGS_DIR}/exports.txt")

file(MAKE_DIRECTORY "${LOGS_DIR}")

# TODO: add support for other platforms
if (WIN32 AND MSVC)
	# Save DLL exports
	add_custom_command(
			TARGET ${LIBRARY_TARGET_NAME} POST_BUILD
			WORKING_DIRECTORY "${CMAKE_CURRENT_BINARY_DIR}"
			COMMENT "Saving DLL file exports"
			BYPRODUCTS "${LOG_EXPORTS_FILE}"

			COMMAND dumpbin /exports $<TARGET_FILE:${LIBRARY_TARGET_NAME}> > "${LOG_EXPORTS_FILE}"

			VERBATIM
	)
endif ()