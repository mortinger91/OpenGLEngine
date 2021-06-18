# glfw
message(STATUS "Configuring GLFW")
set(GLFW_BUILD_DOCS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_TESTS OFF CACHE BOOL "" FORCE)
set(GLFW_INSTALL OFF CACHE BOOL "" FORCE)
add_subdirectory(glfw)
set_target_properties(glfw PROPERTIES FOLDER vendor)

# glm
message(STATUS "Configuring GLM")
set(GLM_QUIET OFF CACHE BOOL "" FORCE)
set(GLM_TEST_ENABLE OFF CACHE BOOL "" FORCE)
add_subdirectory(glm)

# imgui
message(STATUS "Configuring IMGUI")
# set(IMGUI_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)
add_subdirectory(imgui-cmake)
set_target_properties(imgui PROPERTIES FOLDER vendor)

# glew
message(STATUS "Configuring GLEW")
# set(glew-cmake_BUILD_SHARED OFF CACHE BOOL "" FORCE)
set(ONLY_LIBS ON CACHE BOOL "" FORCE)
add_subdirectory(glew-cmake)
set_target_properties(libglew_static PROPERTIES FOLDER vendor)