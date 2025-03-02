cmake_minimum_required(VERSION 3.28.3)

project(VulkanVideo)

file(GLOB_RECURSE sources src/*.cpp include/*.h glfw/src/*.h glfw/build/src/*.h)

# add the data to the target, so it becomes visible in some IDE

add_subdirectory(glfw)

set(CMAKE_CXX_SCAN_FOR_MODULES 0)
set(CMAKE_CXX_STANDARD 20)

find_package(Vulkan REQUIRED)

add_executable(VulkanVideo ${sources})

target_include_directories(VulkanVideo PRIVATE include glfw/src glfw/build/src)
# this lets me include files relative to the root source directory with a <> pair

function(add_wayland_protocol_target TARGET_NAME SPEC NAME)
    add_custom_target(wayland_${TARGET_NAME}_header COMMAND wayland-scanner client-header /usr/share/wayland-protocols${SPEC} ${PROJECT_SOURCE_DIR}/src/${NAME}.h)
    add_dependencies(VulkanVideo wayland_${TARGET_NAME}_header)
    add_custom_target(wayland_${TARGET_NAME}_source COMMAND wayland-scanner private-code /usr/share/wayland-protocols${SPEC} ${PROJECT_SOURCE_DIR}/src/${NAME}.c)
    add_dependencies(VulkanVideo wayland_${TARGET_NAME}_source)
endfunction()

add_wayland_protocol_target(xdg_shell "/stable/xdg-shell/xdg-shell.xml" xdg-shell)


target_link_libraries(VulkanVideo glfw)
target_link_libraries(VulkanVideo wayland-client)
target_link_libraries(VulkanVideo ${Vulkan_LIBRARIES})