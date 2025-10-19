# 🌀 Vulkan Project

A simple Vulkan project written in **C++** and built with **CMake**, designed to run in **Visual Studio Code**.

---

## 📦 Prerequisites

Before building the project, make sure you have the following installed:

### 🔧 Required
- [Vulkan SDK](https://vulkan.lunarg.com/sdk/home) – Provides Vulkan headers, libraries, validation layers, and tools.
- [CMake](https://cmake.org/download/) – Build system generator.
- C++17 or newer compiler – e.g., `clang++`, `g++`, or MSVC.

---

## 🛠️ Building the Project

### Quick Start
```bash
# Configure the build system
cmake -B build

# Build the project
cmake --build build

# Run the application
./build/VulkanApp
```

### Detailed Build Instructions

#### 1. Configure CMake
Generate the build files in a `build` directory:
```bash
cmake -B build
```

This command:
- Creates a `build/` directory if it doesn't exist
- Generates platform-specific build files (Makefiles on Unix, Visual Studio projects on Windows)
- Finds and configures Vulkan SDK and GLFW dependencies
- Sets up automatic shader compilation

#### 2. Build the Project
Compile the source code and shaders:
```bash
cmake --build build
```

This command:
- Compiles all C++ source files
- Automatically compiles GLSL shaders (`.vert`, `.frag`) to SPIR-V (`.spv`) format
- Links the final executable

#### 3. Run the Application
Execute the built Vulkan application:
```bash
./build/VulkanApp
```

### Alternative Build Methods

#### Using Make (Unix/macOS)
```bash
cd build
make
./VulkanApp
```

#### Clean Build
Remove build artifacts and rebuild from scratch:
```bash
rm -rf build
cmake -B build
cmake --build build
```

#### Debug Build
Build with debug symbols:
```bash
cmake -B build -DCMAKE_BUILD_TYPE=Debug
cmake --build build
```

#### Release Build
Build optimized for performance:
```bash
cmake -B build -DCMAKE_BUILD_TYPE=Release
cmake --build build
```

---

## 🎨 Shader Development

The project includes automatic shader compilation. Shaders are located in the `shaders/` directory:
- `simple_shader.vert` - Vertex shader
- `simple_shader.frag` - Fragment shader

**Automatic Compilation:**
- Shaders are automatically compiled when you run `cmake --build build`
- Only modified shaders are recompiled (incremental builds)
- Compiled `.spv` files are generated in the same directory

**Manual Shader Compilation:**
```bash
# Compile vertex shader
glslc shaders/simple_shader.vert -o shaders/simple_shader.vert.spv

# Compile fragment shader
glslc shaders/simple_shader.frag -o shaders/simple_shader.frag.spv
```

---

