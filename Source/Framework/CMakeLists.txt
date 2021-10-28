project(Framework)

CreateTarget(${PROJECT_NAME} EXECUTABLE)

target_link_libraries(${PROJECT_NAME} PRIVATE Networking)

target_link_libraries(${PROJECT_NAME} PRIVATE glfw)
target_link_libraries(${PROJECT_NAME} PRIVATE glad)
target_link_libraries(${PROJECT_NAME} PRIVATE imgui)

target_link_libraries(${PROJECT_NAME} PRIVATE Serialization)

target_include_directories(${PROJECT_NAME} PRIVATE "${GLFW_SOURCE_DIR}/deps/")

find_package(OpenGL REQUIRED)
target_link_libraries(${PROJECT_NAME} PRIVATE ${OPENGL_LIBRARIES})