message(STATUS "   ${Red}Configuring ${PROJECT_NAME} ${VERSION}${ColourReset}")

# Target
add_library(
		${LIBRARY_TARGET_NAME}
		STATIC

		# header files
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
		src/Model.h
		src/vendorLibs.h
		src/ModelLoader.h

		src/libs/stb_image/stb_image.h
		src/tests/Test.h
		src/tests/Test3DCube.h
		src/tests/TestClearColor.h

		# cpp files
		src/Camera.cpp
		src/IndexBuffer.cpp
		src/Material.cpp
		src/Mesh.cpp
		src/Renderer.cpp 
		src/Shader.cpp
		src/Texture.cpp
		src/Utility.cpp
		src/VertexArray.cpp
		src/VertexBuffer.cpp
		src/Model.cpp
		src/ModelLoader.cpp

		src/libs/stb_image/stb_image.cpp
		src/tests/Test.cpp
		src/tests/Test3DCube.cpp
		src/tests/TestClearColor.cpp
)

# Commented copy and symbolic link of res folder, execution is supported only inside the editor
#[[
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
	set (destination "${BUILD_FOLDER}/bin")
	file(COPY ${source} DESTINATION ${destination})
	message(STATUS "   ${Red}copied resources folder from ${source} => ${destination}${ColourReset}")
endif (WIN32)
]]

# Properties
target_include_directories(
		${LIBRARY_TARGET_NAME}
		PUBLIC
		"src"
		"src/libs"
		"res"
		${CMAKE_SOURCE_DIR}/vendor/assimp/include
		${PROJECT_BINARY_DIR}/vendor/assimp/include
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
		
		# Build type macros
		$<$<CONFIG:Debug>:DEBUG>
		$<$<CONFIG:Release>:RELEASE>
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