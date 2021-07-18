#pragma once

#include <optional>
#include <string>

GLuint create_program(GLenum type, std::string shader_text) {
    shader_text = std::string("#version 450\n") + shader_text;
    const char* s[] = {
        shader_text.c_str(),
    };
    return glCreateShaderProgramv(type, 1, s);
}

struct shader {
    GLuint pipeline, program_vertex, program_fragment;
    shader(std::string vertex_source, std::string fragment_source) {
        glGenProgramPipelines(1, &pipeline);
        program_vertex = create_program(GL_VERTEX_SHADER, vertex_source);
        program_fragment = create_program(GL_FRAGMENT_SHADER, fragment_source);
        glUseProgramStages(pipeline, GL_VERTEX_SHADER_BIT, program_vertex);
        glUseProgramStages(pipeline, GL_FRAGMENT_SHADER_BIT, program_fragment);
    }
    void draw() {
        glBindProgramPipeline(pipeline);
    }
};
