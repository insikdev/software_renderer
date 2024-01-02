#pragma once
#include "pipeline.h"

class SimplePipeline : public Pipeline {
public:
    SimplePipeline() = default;
    ~SimplePipeline() = default;

protected:
    void VertexShader(std::vector<Vertex>& vertices) override;
    void FragmentShader() override;
};
