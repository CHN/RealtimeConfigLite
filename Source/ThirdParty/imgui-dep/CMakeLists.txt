cmake_minimum_required(VERSION 3.0)
project(imgui)

set(SOURCE_DIR "${PROJECT_SOURCE_DIR}/imgui")

file(GLOB HEADERS "${SOURCE_DIR}/*.h")
list(APPEND HEADERS "${SOURCE_DIR}/backends/imgui_impl_glfw.h")
list(APPEND HEADERS "${SOURCE_DIR}/backends/imgui_impl_opengl3.h")

file(GLOB SOURCES "${SOURCE_DIR}/*.cpp")
list(APPEND SOURCES "${SOURCE_DIR}/backends/imgui_impl_glfw.cpp")
list(APPEND SOURCES "${SOURCE_DIR}/backends/imgui_impl_opengl3.cpp")

add_library(${PROJECT_NAME} ${HEADERS} ${SOURCES})
target_include_directories(${PROJECT_NAME} PUBLIC ${SOURCE_DIR} "${SOURCE_DIR}/backends/")

target_link_libraries(${PROJECT_NAME} PRIVATE glfw)