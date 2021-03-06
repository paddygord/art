#include <cassert>
#include <iostream>

struct aabb_t {
    int x0, y0, x1, y1;

    aabb_t():
        x0(0), y0(0), x1(0), y1(0) {}
    aabb_t(int x, int y, int width, int height):
        x0(x), y0(y), x1(x + width), y1(y + height)
    {
        assert(!inverted());
    }

    int xpos() {
        return x0;
    }
    int ypos() {
        return y0;
    }
    int width() {
        return x1 - x0;
    }
    int height() {
        return y1 - y0;
    }

    friend std::ostream& operator<<(std::ostream& out, aabb_t x) {
        out << x.x0 << ", ";
        out << x.y0 << ", ";
        out << x.x1 << ", ";
        out << x.y1;
        return out;
    }

    enum class direction {
        left, right, top, bottom, all,
    };
    bool inverted() {
        return !(x1 >= x0 && y1 >= y0);
    }
    bool contains(aabb_t x) {
        return x.x0 >= x0 && x.x1 <= x1 && x.y0 >= y0 && x.y1 <= y1;
    }
    bool overlaps(aabb_t x) {
        return !(x.x1 < x0 || x.x0 > x1 || x.y1 < y0 || x.y0 > y1);
    }
    aabb_t chop(direction d, size_t amount) {
        aabb_t x = *this;
        switch (d) {
            case direction::left:
                x.x1 = x.x0 + amount;
                x0 += amount;
                break;
            case direction::right:
                x.x0 = x.x1 - amount;
                x1 -= amount;
                break;
            case direction::top:
                x.y1 = x.y0 + amount;
                y0 += amount;
                break;
            case direction::bottom:
                x.y0 = x.y1 - amount;
                y1 += amount;
                break;
            case direction::all:
                x.x0 = x0 + amount;
                x.y0 = y0 + amount;
                x.x1 = x1 - amount;
                x.y1 = y1 - amount;
                break;
        }
        assert(!x.inverted());
        assert(!inverted());
        return x;
    }

    void viewport() {
        glViewport(xpos(), ypos(), width(), height());
    }
    void scissor() {
        glEnable(GL_SCISSOR_TEST);
        glScissor(xpos(), ypos(), width(), height());
    }
};
