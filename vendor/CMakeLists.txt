# glfw
message(STATUS "${Red}Configuring GLFW${ColourReset}")
set(GLFW_BUILD_DOCS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_TESTS OFF CACHE BOOL "" FORCE)
set(GLFW_INSTALL OFF CACHE BOOL "" FORCE)
add_subdirectory(glfw)
set_target_properties(glfw PROPERTIES FOLDER vendor)

# glm
message(STATUS "${Red}Configuring GLM${ColourReset}")
set(GLM_QUIET OFF CACHE BOOL "" FORCE)
set(GLM_TEST_ENABLE OFF CACHE BOOL "" FORCE)
add_subdirectory(glm)

# imgui
message(STATUS "${Red}Configuring IMGUI${ColourReset}")
set(IMGUI_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)
add_subdirectory(imgui-cmake)
set_target_properties(imgui PROPERTIES FOLDER vendor)

# glew
message(STATUS "${Red}Configuring GLEW${ColourReset}")
set(glew-cmake_BUILD_SHARED OFF CACHE BOOL "" FORCE)
set(ONLY_LIBS ON CACHE BOOL "" FORCE)
set(USE_GLU OFF CACHE BOOL "" FORCE)
add_subdirectory(glew-cmake)
set_target_properties(libglew_static PROPERTIES FOLDER vendor)

# assimp
message(STATUS "${Red}Configuring ASSIMP${ColourReset}")
set(BUILD_SHARED_LIBS OFF CACHE BOOL "" FORCE)
set(ASSIMP_BUILD_ASSIMP_TOOLS OFF CACHE BOOL "" FORCE)
set(ASSIMP_NO_EXPORT ON CACHE BOOL "" FORCE)
set(ASSIMP_BUILD_TESTS OFF CACHE BOOL "" FORCE)
set(ASSIMP_INSTALL OFF CACHE BOOL "" FORCE)
set(ASSIMP_BUILD_DRACO OFF CACHE BOOL "" FORCE)
add_subdirectory(assimp)
set_target_properties(assimp PROPERTIES FOLDER vendor)

message(STATUS "   ${Yellow}Finished configuring vendor libraries${ColourReset}")