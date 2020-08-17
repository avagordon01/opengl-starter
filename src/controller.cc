#include "controller.hh"

#define GL_GLEXT_PROTOTYPES
#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <Eigen/Dense>

controller::controller() {
    GLFWwindow* window = glfwGetCurrentContext();

    glfwSetKeyCallback(window, key_callback_wrapper);
    glfwSetCursorPosCallback(window, cursor_pos_callback_wrapper);
    glfwSetMouseButtonCallback(window, mouse_button_callback_wrapper);
    glfwSetScrollCallback(window, scroll_callback_wrapper);

    glfwSetWindowUserPointer(window, static_cast<void *>(this));

    glfwSetTime(0);
}

void controller::key_callback_wrapper(GLFWwindow* window, int key, int scancode, int action, int mods) {
    auto* me = static_cast<controller*>(glfwGetWindowUserPointer(window));
    me->key_callback(window, key, scancode, action, mods);
}
void controller::cursor_pos_callback_wrapper(GLFWwindow* window, const double x, const double y) {
    auto *me = static_cast<controller*>(glfwGetWindowUserPointer(window));
    me->cursor_pos_callback(window, x, y);
}
void controller::mouse_button_callback_wrapper(GLFWwindow* window, int button, int action, int mods) {
    auto *me = static_cast<controller*>(glfwGetWindowUserPointer(window));
    me->mouse_button_callback(window, button, action, mods);
}
void controller::scroll_callback_wrapper(GLFWwindow* window, double dx, double dy) {
    auto* me = static_cast<controller*>(glfwGetWindowUserPointer(window));
    me->scroll_callback(window, dx, dy);
}

void controller::key_callback(GLFWwindow* window, int key, int scancode, int action, int mods) {
    float speed = 0;
    if (action == GLFW_PRESS) {
        speed = 10;
    } else if (action == GLFW_RELEASE) {
        speed = -10;
    }

    switch (key) {
    case GLFW_KEY_W:
        camera_velocity.z() -= speed;
        break;
    case GLFW_KEY_A:
        camera_velocity.x() -= speed;
        break;
    case GLFW_KEY_S:
        camera_velocity.z() += speed;
        break;
    case GLFW_KEY_D:
        camera_velocity.x() += speed;
        break;
    case GLFW_KEY_SPACE:
        camera_velocity.y() += speed;
        break;
    case GLFW_KEY_LEFT_CONTROL:
        camera_velocity.y() -= speed;
        break;
    }

    if (key == GLFW_KEY_ESCAPE || key == GLFW_KEY_Q) {
        glfwSetWindowShouldClose(window, true);
    }
}
void controller::cursor_pos_callback(GLFWwindow* window, const double x, const double y) {
    int width, height;
    glfwGetWindowSize(window, &width, &height);
    double mousex = x / width - 0.5;
    double mousey = 0.5 - y / height;

    mousex *= 4.0;
    mousey *= 4.0;

    camera_orientation = Eigen::AngleAxisf(mousex, Eigen::Vector3f::UnitY()) * Eigen::AngleAxisf(-mousey, Eigen::Vector3f::UnitX());
    camera_orientation.normalize();
}
void controller::mouse_button_callback(GLFWwindow* window, int button, int action, int mods) {
}
void controller::scroll_callback(GLFWwindow* window, double dx, double dy) {
    camera_pos.z() -= dy;
}

Eigen::Matrix4f perspective_matrix(float y_fov, float aspect_ratio, float n, float f) {
    Eigen::Matrix4f m = Eigen::Matrix4f::Zero();
    const float a = 1.f / tan((y_fov * 2 * M_PI / 360) / 2.f);
    m(0, 0) = a / aspect_ratio;
    m(1, 1) = a;
    m(2, 2) = -((f + n) / (f - n));
    m(3, 2) = -((2.f * f * n) / (f - n));
    m(2, 3) = -1.0f;
    return m;
}

Eigen::Matrix4f controller::tick() {
    glfwPollEvents();
    GLFWwindow* window = glfwGetCurrentContext();
    if (glfwWindowShouldClose(window)) {
        glfwDestroyWindow(window);
        exit(EXIT_SUCCESS);
    }

    float current_time = glfwGetTime();
    float dt = current_time - previous_time;
    previous_time = current_time;
    camera_pos += dt * camera_velocity;

    Eigen::Affine3f view = Eigen::Affine3f::Identity();
    view.translation() = -camera_pos;
    view.linear() = camera_orientation.toRotationMatrix();

    int window_width, window_height;
    glfwGetWindowSize(window, &window_width, &window_height);
    float aspect_ratio = static_cast<float>(window_width) / window_height;
    Eigen::Matrix4f projection = perspective_matrix(60, aspect_ratio, 1, 100);

    Eigen::Matrix4f mvp = projection * view.matrix();

    return mvp;
}
