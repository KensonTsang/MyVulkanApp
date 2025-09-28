cmake_minimum_required(VERSION 3.10)

# Project Name
project(MyVulkanApp)

# Set C++ standard to 17
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Find the Vulkan package provided by the SDK
find_package(Vulkan REQUIRED)

# Find the GLFW package installed by Homebrew
find_package(glfw3 REQUIRED)

# Add our executable from all source files
add_executable(VulkanApp 
    src/main.cpp
    src/lve_window.cpp
    src/first_app.cpp
    src/lve_pipline.cpp
)

# Add include directory
target_include_directories(VulkanApp PRIVATE include)

# Link the Vulkan and GLFW libraries to our app
target_link_libraries(VulkanApp PUBLIC Vulkan::Vulkan glfw)