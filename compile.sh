glslc shaders/vsShader.vert -o vert.spv
glslc shaders/fsShader.frag -o frag.spv

mv *.spv shaders