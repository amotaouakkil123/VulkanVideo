cmake_minimum_required(VERSION 3.28.3)

project(VulkanVideo)

file(GLOB_RECURSE sources src/*.cpp include/*.h glfw/src/*.h)

# add the data to the target, so it becomes visible in some IDE

add_subdirectory(glfw)

set(CMAKE_CXX_SCAN_FOR_MODULES 0)
set(CMAKE_CXX_STANDARD 20)

find_package(Vulkan REQUIRED)

add_executable(VulkanVideo ${sources})

target_include_directories(VulkanVideo PRIVATE include)
# this lets me include files relative to the root source directory with a <> pair
target_link_libraries(VulkanVideo glfw)
target_link_libraries(VulkanVideo ${Vulkan_LIBRARIES})