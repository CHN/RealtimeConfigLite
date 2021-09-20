project(ThirdParty)

CreateTarget(${PROJECT_NAME} LIBRARY)

add_subdirectory(glfw)
add_subdirectory(glad)
add_subdirectory(imgui-dep)