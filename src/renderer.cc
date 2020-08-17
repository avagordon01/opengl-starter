#include "renderer.hh"

#define GL_GLEXT_PROTOTYPES
#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <Eigen/Dense>

#include <vector>

void error_callback(int error, const char* description) {
    (void)error;
    printf("glfw error: %s\n", description);
    exit(EXIT_FAILURE);
}

#define VERSION "#version 330 core\n"
#define QUOTE(...) #__VA_ARGS__
static const char* point_vertex_shader_text = VERSION QUOTE(
    uniform mat4 mvp;
    in vec3 colour;
    in vec3 position;
    out vec3 out_colour;
    void main() {
        gl_Position = mvp * vec4(position, 1.0);
        out_colour = colour;
    }
);

static const char* point_fragment_shader_text = VERSION QUOTE(
    in vec3 colour;
    out vec4 out_colour;
    void main() {
        out_colour = vec4(colour, 1.0);
    }
);

renderer::renderer() {
    glfwSetErrorCallback(error_callback);
    if (!glfwInit()) {
        exit(EXIT_FAILURE);
    }
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_DOUBLEBUFFER, 1);
    glfwWindowHint(GLFW_SAMPLES, 4);
    GLFWwindow* window = glfwCreateWindow(800, 600, "demo", nullptr, nullptr);
    if (!window) {
        glfwTerminate();
        exit(EXIT_FAILURE);
    }
    glfwMakeContextCurrent(window);
    glfwSwapInterval(1);

    glewExperimental = GL_TRUE;
    glewInit();

    char error_log[4096] = "";
    program_vertex = glCreateShaderProgramv(GL_VERTEX_SHADER, 1, &point_vertex_shader_text);
    glGetProgramInfoLog(program_vertex, sizeof(error_log), nullptr, error_log);
    if (error_log[0] != 0) {
        printf("%s\n", error_log);
        exit(EXIT_FAILURE);
    }
    GLuint program_fragment = glCreateShaderProgramv(GL_FRAGMENT_SHADER, 1, &point_fragment_shader_text);
    glGetProgramInfoLog(program_fragment, sizeof(error_log), nullptr, error_log);
    if (error_log[0] != 0) {
        printf("%s\n", error_log);
        exit(EXIT_FAILURE);
    }

    glGenProgramPipelines(1, &pipeline);
    glUseProgramStages(pipeline, GL_VERTEX_SHADER_BIT, program_vertex);
    glUseProgramStages(pipeline, GL_FRAGMENT_SHADER_BIT, program_fragment);

    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);

    glGenBuffers(1, &buffer_vertices);
    glBindBuffer(GL_ARRAY_BUFFER, buffer_vertices);
    glBufferData(GL_ARRAY_BUFFER, 0, nullptr, GL_DYNAMIC_DRAW);

    GLint location_position = glGetAttribLocation(program_vertex, "position");
    glEnableVertexAttribArray(location_position);
    glVertexAttribPointer(
        location_position, 3, GL_FLOAT, GL_FALSE,
        sizeof(struct vertex),
        (void*)offsetof(struct vertex, position)
    );

    GLint location_colour = glGetAttribLocation(program_vertex, "colour");
    glEnableVertexAttribArray(location_colour);
    glVertexAttribPointer(
        location_colour, 3, GL_FLOAT, GL_FALSE,
        sizeof(struct vertex),
        (void*)offsetof(struct vertex, colour)
    );

    glEnable(GL_DEPTH_TEST);
    glEnable(GL_DEPTH_CLAMP);
    glDepthFunc(GL_LEQUAL);
    glEnable(GL_BLEND);
    glEnable(GL_MULTISAMPLE);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
}

void renderer::render(std::vector<vertex> &vertices, Eigen::Matrix4f mvp) {
    int window_width, window_height;
    glfwGetWindowSize(glfwGetCurrentContext(), &window_width, &window_height);
    glViewport(0, 0, window_width, window_height);

    glClearColor(0.0, 0.0, 0.0, 1.0);
    glClearDepth(1);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glBindVertexArray(vao);
    glBindBuffer(GL_ARRAY_BUFFER, buffer_vertices);
    glBufferData(
        GL_ARRAY_BUFFER, sizeof(struct vertex) * vertices.size(),
        vertices.data(), GL_DYNAMIC_DRAW
    );
    glBindProgramPipeline(pipeline);
    GLint location_mvp = glGetUniformLocation(program_vertex, "mvp");
    glProgramUniformMatrix4fv(
        program_vertex, location_mvp, 1,
        GL_FALSE, reinterpret_cast<const GLfloat*>(mvp.data())
    );
    glDrawArrays(GL_TRIANGLES, 0, vertices.size());

    glfwSwapBuffers(glfwGetCurrentContext());
}
