cmake_minimum_required(VERSION 3.28.3)

project(VulkanVideo)

file(GLOB_RECURSE sources      src/*.cpp src/*.h)

# add the data to the target, so it becomes visible in some IDE

add_subdirectory(glfw)

# just for example add some compiler flags
target_compile_options(VulkanVideo PUBLIC -std=c++20)


find_package(Vulkan REQUIRED)

# this lets me include files relative to the root source directory with a <> pair
add_executable(VulkanVideo ${sources})
target_link_libraries(VulkanVideo glfw)