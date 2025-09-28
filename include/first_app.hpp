#pragma once

#include "lve_window.hpp"
#include "lve_pipeline.hpp"

namespace lve {

    class FirstApp {    
    public:

        void run();
    
        static constexpr int WIDTH = 800;
        static constexpr int HEIGHT = 600;

    private:
        LveWindow lveWindow{WIDTH, HEIGHT, "Hello Vulkan!"};
        LvePipline lvePipline{"../shaders/simple_shader.vert.spv", "../shaders/simple_shader.frag.spv"};
    };

}