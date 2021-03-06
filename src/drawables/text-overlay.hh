#include <string>
#include "engine/font-atlas.hh"
#include "engine/drawable.hh"

struct text_overlay {
    struct char_vertex {
        std::array<float, 2> vertex;
        std::array<float, 3> texcoords;
    };
    monospace_unicode_font_atlas& atlas;
    drawable<char_vertex> drawable;
    void gen_text(std::wstring text, std::array<float, 2> position) {
        auto[x, y] = position;
        for (uint32_t c: text) {
            float char_index = c;
            float w = atlas.width();
            float h = atlas.height();
            drawable.vbo.data.push_back({{x,     y + h}, {0.0f, 0.0f, char_index}});
            drawable.vbo.data.push_back({{x,     y    }, {0.0f, h,    char_index}});
            drawable.vbo.data.push_back({{x + w, y    }, {w,    h,    char_index}});

            drawable.vbo.data.push_back({{x,     y + h}, {0.0f, 0.0f, char_index}});
            drawable.vbo.data.push_back({{x + w, y    }, {w,    h,    char_index}});
            drawable.vbo.data.push_back({{x + w, y + h}, {w,    0.0f, char_index}});
            x += w;
        }
    }
    text_overlay(monospace_unicode_font_atlas& atlas_):
    atlas(atlas_),
    drawable(
        R"foo(
in vec2 vertex;
in vec3 texcoords;
out vec3 texcoords_f;

out gl_PerVertex {
    vec4 gl_Position;
};
void main() {
    gl_Position = projection * view * vec4(vertex, 0.0f, 1.0f);
    texcoords_f = texcoords;
}
)foo",
        atlas.header_shader_text + R"foo(
in vec3 texcoords_f;
out vec4 colour;

void main() {
    colour = vec4(bitmap_font_atlas_fetch(ivec3(texcoords_f)));
}
)foo")
    {
        drawable.vbo.bind(drawable.shader.program_vertex, "vertex", &char_vertex::vertex);
        drawable.vbo.bind(drawable.shader.program_vertex, "texcoords", &char_vertex::texcoords);
        atlas.bind(drawable.shader.program_fragment);
    }
    void draw() {
        drawable.draw(GL_TRIANGLES);
    }
};
