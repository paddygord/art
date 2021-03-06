#include "engine/drawable.hh"

struct grid {
    drawable<> drawable;
    grid():
        drawable(fullscreen_quad{}.vertex_shader,
        R"foo(
layout(pixel_center_integer) in vec4 gl_FragCoord;
out vec4 colour;

void main() {
    ivec2 fc = ivec2(gl_FragCoord.xy);

    ivec2 fc_mod = fc % ivec2(64);

    bool x = (fc_mod.x == 0);
    bool y = (fc_mod.y == 0);
    colour = vec4(vec3(1), 0.25) * float(x || y);

    bool x_plus = x && (fc_mod.y < 8 || fc_mod.y > 56);
    bool y_plus = y && (fc_mod.x < 8 || fc_mod.x > 56);
    colour += vec4(1) * float(x_plus || y_plus);
}
)foo")
    {}
    void draw() {
        drawable.draw(GL_QUADS);
    }
};
