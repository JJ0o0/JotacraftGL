#include <player.hpp>
#include <world/block_registry.hpp>

void Player::Update(World& world, float deltatime) {
    m_velocity.y += GRAVITY * deltatime;

    float oldX = m_position.x;
    m_position.x += m_velocity.x * deltatime;

    if (collidesWithWorld(world, GetAABB())) {
        m_position.x = oldX;
        m_velocity.x = 0.0f;
    }

    m_grounded = false;
    float oldY = m_position.y;
    m_position.y += m_velocity.y * deltatime;

    if (collidesWithWorld(world, GetAABB())) {
        if (m_velocity.y < 0) { 
            m_grounded = true; 
            m_position.y = std::floor(GetAABB().Minimum.y) + 1.0f;
        } else if (m_velocity.y > 0) {
            m_position.y = std::floor(GetAABB().Maximum.y) - HEIGHT;
        }

        m_velocity.y = 0.0f;
    }

    float oldZ = m_position.z;
    m_position.z += m_velocity.z * deltatime;

    if (collidesWithWorld(world, GetAABB())) {
        m_position.z = oldZ;
        m_velocity.z = 0.0f;
    }
}

void Player::HandleMovement(GLFWwindow* window, float deltatime) {
    if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS) {
        m_speed = SPRINT_SPEED;
    } else {
        m_speed = WALK_SPEED;
    }
    
    auto cameraFront = m_camera.GetFront();
    glm::vec3 front = glm::normalize(glm::vec3(cameraFront.x, 0, cameraFront.z));

    auto right = m_camera.GetRight();

    glm::vec3 direction{0.0f};
    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) direction += front;
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) direction -= front;
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) direction += right;
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) direction -= right; 

    if (glm::length(direction) > 0.0f) direction = glm::normalize(direction);

    glm::vec3 targetVelocity = direction * m_speed;

    float friction = m_grounded ? GROUND_FRICTION : AIR_FRICTION;
    m_velocity.x *= std::pow(friction, deltatime);
    m_velocity.z *= std::pow(friction, deltatime);

    float accel = m_grounded ? GROUND_ACCEL : AIR_ACCEL;
    m_velocity.x += direction.x * accel * deltatime;
    m_velocity.z += direction.z * accel * deltatime;

    glm::vec2 horizontal{m_velocity.x, m_velocity.z};
    if (glm::length(horizontal) > m_speed) {
        horizontal = glm::normalize(horizontal) * m_speed;

        m_velocity.x = horizontal.x;
        m_velocity.z = horizontal.y;
    }

    if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS and m_grounded) {
        m_velocity.y = JUMP_FORCE;
    }
}

RaycastResult Player::Raycast(World& world, float maxDistance) const {
    glm::vec3 origin = GetEyePosition();
    glm::vec3 direction = m_camera.GetFront();

    glm::ivec3 voxel {
        std::floor(origin.x),
        std::floor(origin.y),
        std::floor(origin.z),
    };

    glm::ivec3 step;
    glm::vec3 tMax;
    glm::vec3 tDelta;

    for (int i = 0; i < 3; i++) {
        if (direction[i] > 0) {
            step[i] = 1;
            tMax[i] = (std::floor(origin[i]) + 1.0f - origin[i]) / direction[i];
            tDelta[i] = 1.0f / direction[i];
        } else if (direction[i] < 0) {
            step[i] = -1;
            tMax[i] = (origin[i] - std::floor(origin[i])) / -direction[i];
            tDelta[i] = -1.0f / direction[i];
        } else {
            step[i] = 0;
            tMax[i] = std::numeric_limits<float>::infinity();
            tDelta[i] = std::numeric_limits<float>::infinity();
        }
    }

    glm::ivec3 previousVoxel = voxel;

    while (true) {
        if (tMax.x < tMax.y && tMax.x < tMax.z) {
            if (tMax.x > maxDistance) break;
            previousVoxel = voxel;
            voxel.x += step.x;
            tMax.x += tDelta.x;
        } else if (tMax.y < tMax.z) {
            if (tMax.y > maxDistance) break;
            previousVoxel = voxel;
            voxel.y += step.y;
            tMax.y += tDelta.y;
        } else {
            if (tMax.z > maxDistance) break;
            previousVoxel = voxel;
            voxel.z += step.z;
            tMax.z += tDelta.z;
        }

        if (world.GetBlock(voxel.x, voxel.y, voxel.z) != BlockType::Air) {
            return RaycastResult{true, voxel, previousVoxel};
        }
    }

    return RaycastResult{false};
}

AABB Player::GetAABB() const {
    glm::vec3 halfExtents{WIDTH / 2.0f, 0.0f, WIDTH / 2.0f};
    
    return AABB{
        m_position - halfExtents,
        m_position + glm::vec3{halfExtents.x, HEIGHT, halfExtents.z}
    };
}

glm::vec3 Player::GetEyePosition() const {
    return m_position + glm::vec3(0, EYE_HEIGHT, 0);
}

bool Player::collidesWithWorld(World& world, const AABB& box) const {
    glm::ivec3 min {
        static_cast<int>(std::floor(box.Minimum.x)),
        static_cast<int>(std::floor(box.Minimum.y)),
        static_cast<int>(std::floor(box.Minimum.z)),
    };

    glm::ivec3 max {
        static_cast<int>(std::floor(box.Maximum.x)),
        static_cast<int>(std::floor(box.Maximum.y)),
        static_cast<int>(std::floor(box.Maximum.z)),
    };

    for (int x = min.x; x <= max.x; x++) {
        for (int y = min.y; y <= max.y; y++) {
            for (int z = min.z; z <= max.z; z++) {
                BlockType block = world.GetBlock(x, y, z);
                if (BlockRegistry::Get(block).Solid) return true;
            }
        }
    }

    return false;
}