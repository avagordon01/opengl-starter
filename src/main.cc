#include "controller.hh"
#include "renderer.hh"

#include <random>

int main() {
    renderer renderer{};
    controller controller{};

    std::vector<vertex> vertices {
        {{0, 0, 0}, {1, 0, 0}},
        {{0, 10, 0}, {0, 1, 0}},
        {{10, 0, 0}, {0, 0, 1}},
    };
    std::mt19937 engine(0xfeed);
    std::uniform_real_distribution<float> pd(0, 10);
    for (size_t i = 0; i < 1000; i++) {
        vertices.push_back({{pd(engine), pd(engine), pd(engine)}, {1, 0, 0}});
        vertices.push_back({{pd(engine), pd(engine), pd(engine)}, {0, 1, 0}});
        vertices.push_back({{pd(engine), pd(engine), pd(engine)}, {0, 0, 1}});
    }

    while (true) {
        renderer.render(vertices, controller.tick());
    }
    return EXIT_SUCCESS;
}
