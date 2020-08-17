#define GL_GLEXT_PROTOTYPES
#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <Eigen/Dense>

#include <vector>

struct vertex {
    struct { float x, y, z; } position;
    struct { float r, g, b; } colour;
};

struct renderer {
    renderer();
    void render(std::vector<vertex> &vertices, Eigen::Matrix4f mvp);

private:
    GLuint vao;
    GLuint buffer_vertices;
    GLuint pipeline;
    GLuint program_vertex;
};
