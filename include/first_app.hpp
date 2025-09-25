#pragma once

#include "lve_window.hpp"

namespace lve {

    class FirstApp {    
    public:

        void run();
    
        static constexpr int WIDTH = 800;
        static constexpr int HEIGHT = 600;

    private:
        LveWindow lveWindow{WIDTH, HEIGHT, "Hello Vulkan!"};

    };

}