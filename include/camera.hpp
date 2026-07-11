#pragma once

#include <GLFW/glfw3.h>
#include <glm/glm.hpp>

struct CameraProperties {
    // CAMERA
    float FieldOfView = 90.0f;
    float AspectRatio = 4.0f / 3.0f;
    float NearPlane = 0.1f;
    float FarPlane = 100.0f;

    // MOVEMENT
    float WalkSpeed = 3.0f;
    float RunSpeed = 6.0f;

    // MOUSE
    float MouseSensitivity = 0.15f;
    float MinimumPitchClamp = -89.0f;
	float MaximumPitchClamp = 89.0f;
};

class Camera {
    public:
        Camera(const CameraProperties& properties = {}) : m_properties(properties) {}

        void HandleMouseMovement(double xpos, double ypos);
        void HandleMovementInput(GLFWwindow* window, float dt);
        void ResetMouseMovement() { m_firstMouse = true; }

        glm::vec3 GetPosition() const { return m_position; }
        glm::vec3 GetRight() const;
        glm::mat4 GetViewMatrix() const;
        glm::mat4 GetProjectionMatrix() const;
    private:
        CameraProperties m_properties;

        bool m_firstMouse = true;

        float m_yaw = -90.0f;
		float m_pitch = 0.0f;
        float m_speed = 0.0f;

		glm::vec2 m_lastMousePosition{0.0f};

        glm::vec3 m_position = { 0, 0, 3};
        glm::vec3 m_front = { 0, 0, -1};
        glm::vec3 m_up = { 0, 1, 0};
};