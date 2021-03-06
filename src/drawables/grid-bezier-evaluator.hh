#include "engine/drawable.hh"
#include "util/misc.hh"

template<typename T, size_t Order>
struct bezier_evaluator {
    std::array<T, (Order + 1) * (Order + 1)> controls;

    std::array<float, Order + 1> bernstein_coefficients(float x) {
        std::array<float, Order + 1> b;
        if (Order == 0) {
            b[0] = 1.0;
        } else {
            b[0] = 1.0 - x;
            b[1] = x;

            for (size_t i = 2; i <= Order; i++) {
                b[i] = x * b[i-1];
                for (size_t j = i - 1; 1 <= j; j--) {
                    b[j] = x * b[j-1] + (1.0 - x) * b[j];
                }
                b[0] = (1.0 - x) * b[0];
            }
        }
        return b;
    }
    std::array<float, 3> eval_coord(std::array<float, 2> x) {
        auto us = bernstein_coefficients(x[0]);
        auto vs = bernstein_coefficients(x[1]);
        T result{0};
        for (size_t i = 0; i < Order + 1; i++) {
            for (size_t j = 0; j < Order + 1; j++) {
                result += us[i] * vs[j] * controls[i + j * (Order + 1)];
            }
        }
        return vec_to_array(result);
    }
};

template<size_t Order, typename T>
struct grid_bezier_renderer {
    GLenum type;
    drawable<> drawable;
    bezier_evaluator<T, Order> be;
    size_t n, m;
    grid_bezier_renderer(GLenum mode, size_t n_, size_t m_):
        type(primitive_mode_to_primitive_type(mode)),
        drawable(),
        n(n_), m(m_)
    {
        drawable.vbo.bind(drawable.shader.program_vertex, "vertex");
    }
    std::array<float, 2> grid_cvt(std::array<size_t, 2> x) {
        return {
            n == 0 ? 0 : static_cast<float>(x[0]) / n,
            m == 0 ? 0 : static_cast<float>(x[1]) / m,
        };
    }
    void draw() {
        drawable.vbo.data.clear();
        if (type == GL_TRIANGLES) {
            for (size_t j = 0; j < m; j++) {
                for (size_t i = 0; i <= n; i++) {
                    drawable.vbo.data.push_back(be.eval_coord(grid_cvt({i, j})));
                    drawable.vbo.data.push_back(be.eval_coord(grid_cvt({i, j + 1})));
                    drawable.vbo.data.push_back(be.eval_coord(grid_cvt({i + 1, j})));

                    drawable.vbo.data.push_back(be.eval_coord(grid_cvt({i + 1, j + 1})));
                    drawable.vbo.data.push_back(be.eval_coord(grid_cvt({i + 1, j})));
                    drawable.vbo.data.push_back(be.eval_coord(grid_cvt({i, j + 1})));
                }
            }
        } else if (type == GL_LINES) {
            for (size_t j = 0; j <= m; j++) {
                for (size_t i = 0; i < n; i++) {
                    drawable.vbo.data.push_back(be.eval_coord(grid_cvt({i, j})));
                    drawable.vbo.data.push_back(be.eval_coord(grid_cvt({i + 1, j})));
                }
            }

            for (size_t i = 0;  i <= n; i++) {
                for (size_t j = 0; j < m; j++) {
                    drawable.vbo.data.push_back(be.eval_coord(grid_cvt({i, j})));
                    drawable.vbo.data.push_back(be.eval_coord(grid_cvt({i, j + 1})));
                }
            }
        } else if (type == GL_POINTS) {
            for (size_t j = 0; j <= m; j++) {
                for (size_t i = 0; i <= n; i++) {
                    drawable.vbo.data.push_back(be.eval_coord(grid_cvt({i, j})));
                }
            }
        }
        drawable.draw(type);
    }
};
