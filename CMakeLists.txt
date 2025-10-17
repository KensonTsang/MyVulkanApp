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
    src/lve_pipeline.cpp
    src/lve_device.cpp
    src/lve_swap_chain.cpp
)

# Add include directory
target_include_directories(VulkanApp PRIVATE include)

# Link the Vulkan and GLFW libraries to our app
target_link_libraries(VulkanApp PUBLIC Vulkan::Vulkan glfw)

# Shader compilation setup
set(GLSL_VALIDATOR "${Vulkan_INCLUDE_DIRS}/../bin/glslc")

# Find all shader files
file(GLOB_RECURSE GLSL_SOURCE_FILES
    "${PROJECT_SOURCE_DIR}/shaders/*.frag"
    "${PROJECT_SOURCE_DIR}/shaders/*.vert"
)

# Create custom target for shader compilation
foreach(GLSL ${GLSL_SOURCE_FILES})
    get_filename_component(FILE_NAME ${GLSL} NAME)
    set(SPIRV "${PROJECT_SOURCE_DIR}/shaders/${FILE_NAME}.spv")
    add_custom_command(
        OUTPUT ${SPIRV}
        COMMAND ${GLSL_VALIDATOR} ${GLSL} -o ${SPIRV}
        DEPENDS ${GLSL}
        COMMENT "Compiling ${FILE_NAME}"
    )
    list(APPEND SPIRV_BINARY_FILES ${SPIRV})
endforeach(GLSL)

# Create a target that depends on all shader files
add_custom_target(
    Shaders 
    DEPENDS ${SPIRV_BINARY_FILES}
    COMMENT "Compiling shaders"
)

# Make sure shaders are compiled before the main executable
add_dependencies(VulkanApp Shaders)