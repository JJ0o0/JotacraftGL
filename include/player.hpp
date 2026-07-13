#pragma once

#include <camera.hpp>
#include <world/world.hpp>
#include <glm/glm.hpp>

struct AABB {
    glm::vec3 Minimum;
    glm::vec3 Maximum;

    bool Intersects(const AABB& other) const {
        return (
            Maximum.x > other.Minimum.x &&
            Minimum.x < other.Maximum.x &&

            Maximum.y > other.Minimum.y &&
            Minimum.y < other.Maximum.y &&

            Maximum.z > other.Minimum.z &&
            Minimum.z < other.Maximum.z
        );
    }
};

struct RaycastResult {
    bool Hit = false;
    glm::ivec3 VoxelPosition{0};
    glm::ivec3 PreviousVoxelPosition{0};
};

class Player {
    public:
        static constexpr float WALK_SPEED = 4.317f;
        static constexpr float SPRINT_SPEED = 5.612f;
        static constexpr float JUMP_FORCE = 7.07f;

        void Update(World& world, float deltatime);
        void HandleMovement(GLFWwindow* window, float deltatime);

        RaycastResult Raycast(World& world, float maxDistance) const;
        AABB GetAABB() const;

        glm::vec3 GetEyePosition() const;

        void SetPosition(const glm::vec3& position) { m_position = position; }
        glm::vec3 GetPosition() const { return m_position; }

        glm::vec3 GetVelocity() const { return m_velocity; }

        Camera& GetCamera() { return m_camera; }
        const Camera& GetCamera() const { return m_camera; }
    private:
        Camera m_camera{};

        float m_speed = 0.0f;
        bool m_grounded = false;
        glm::vec3 m_position{0, 20, 3};
        glm::vec3 m_velocity{0.0f};

        static constexpr float WIDTH = 0.6f;
        static constexpr float HEIGHT = 1.8f;
        static constexpr float EYE_HEIGHT = 1.62f;
        static constexpr float GRAVITY = -20.0f;
        static constexpr float GROUND_FRICTION = 1.0f / 100000.0f;
        static constexpr float AIR_FRICTION = 1.0f / 1000.0f;
        static constexpr float GROUND_ACCEL = 60.0f;
        static constexpr float AIR_ACCEL = 18.0f;

        bool collidesWithWorld(World& world, const AABB& box) const;
};