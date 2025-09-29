#pragma once

#include "lve_device.hpp"

#include <string>
#include <vector>

namespace lve {


    struct PipelineConfigInfo {

    };

    class LvePipline {
        public:
            LvePipline(LveDevice& device,
                        const std::string& vertFilepath,
                        const std::string& fragFilepath,
                        const PipelineConfigInfo& configInfo);


            ~LvePipline(){}

            LvePipline(const LvePipline&) = delete;
            void operator=(const LvePipline&) = delete;

            static PipelineConfigInfo defaultPipelineConfigInfo(uint32_t width, uint32_t height);


        private:
            static std::vector<char> readFile(const std::string& filepath);
            void createGraphicsPipeline(const std::string& vertFilepath,
                                        const std::string& fragFilepath,
                                        const PipelineConfigInfo& configInfo);



            void createShaderModule(const std::vector<char>& code, VkShaderModule* shaderModule);
                

            LveDevice& lveDevice;
            VkPipeline grahicsPipline;
            VkShaderModule vertShaderModule;
            VkShaderModule fragShaderModule;
    };
}