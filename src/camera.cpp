#include <camera.hpp>
#include <GLFW/glfw3.h>
#include <glm/ext/matrix_clip_space.hpp>
#include <glm/ext/matrix_transform.hpp>
#include <algorithm>

void Camera::HandleMouseMovement(double xpos, double ypos) {
    if (!m_focused) {
        m_firstMouse = true;
        return; 
    }

    if (m_firstMouse) {
        m_lastMousePosition.x = xpos;
        m_lastMousePosition.y = ypos;
        m_firstMouse = false;
    }

    glm::vec2 offset{ 
        xpos - m_lastMousePosition.x,
        m_lastMousePosition.y - ypos,
    };

    m_lastMousePosition.x = xpos;
    m_lastMousePosition.y = ypos;

    offset *= m_properties.MouseSensitivity;

    m_yaw += offset.x;
    m_pitch += offset.y;
    m_pitch = std::clamp(m_pitch, -89.0f, 89.0f);

    glm::vec3 direction;
    direction.x = cos(glm::radians(m_yaw)) * cos(glm::radians(m_pitch));
    direction.y = sin(glm::radians(m_pitch));
    direction.z = sin(glm::radians(m_yaw)) * cos(glm::radians(m_pitch));

    m_front = glm::normalize(direction);
}

void Camera::HandleMovementInput(GLFWwindow* window, float dt) {
    if (!m_focused) return;

    float speed = 0.0f;

    if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS) {
        speed = m_properties.RunSpeed * dt;
    } else {
        speed = m_properties.WalkSpeed * dt;
    }

    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) m_position += m_front * speed;
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) m_position -= m_front * speed;
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) m_position += GetRight() * speed;
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) m_position -= GetRight() * speed;
    if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS) m_position += m_up * speed;
    if (glfwGetKey(window, GLFW_KEY_LEFT_CONTROL) == GLFW_PRESS) m_position -= m_up * speed;
}

void Camera::HandleMouseInput(GLFWwindow* window) {
    if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_RIGHT) == GLFW_PRESS) {
        m_focused = true;
    } else if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_RIGHT) == GLFW_RELEASE) {
        m_focused = false;
    }

    updateFocus(window);
}

glm::vec3 Camera::GetRight() const {
    return glm::normalize(glm::cross(m_front, m_up));
}

glm::mat4 Camera::GetViewMatrix() const {
    return glm::lookAt(
        m_position,
        m_position + m_front,
        m_up
    );
}

glm::mat4 Camera::GetProjectionMatrix() const {
    return glm::perspective(
        glm::radians(m_properties.FieldOfView),
        m_properties.AspectRatio,
        m_properties.NearPlane,
        m_properties.FarPlane
    );
};

void Camera::updateFocus(GLFWwindow* window) {
    if (m_focused) glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    else glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
}