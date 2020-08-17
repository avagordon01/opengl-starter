#define GL_GLEXT_PROTOTYPES
#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <Eigen/Dense>

struct controller {
    controller();
    Eigen::Matrix4f tick();

private:
    static void key_callback_wrapper(GLFWwindow* window, int key, int scancode, int action, int mods);
    static void cursor_pos_callback_wrapper(GLFWwindow* window, const double x, const double y);
    static void mouse_button_callback_wrapper(GLFWwindow* window, int button, int action, int mods);
    static void scroll_callback_wrapper(GLFWwindow* window, double dx, double dy);

    void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods);
    void cursor_pos_callback(GLFWwindow* window, const double x, const double y);
    void mouse_button_callback(GLFWwindow* window, int button, int action, int mods);
    void scroll_callback(GLFWwindow* window, double dx, double dy);

    double previous_time {0.0};
    Eigen::Vector3f camera_pos {0, 0, 16};
    Eigen::Vector3f camera_velocity {0, 0, 0};
    Eigen::Quaternionf camera_orientation {Eigen::Quaternionf::Identity()};
};
