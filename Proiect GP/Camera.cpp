#include "Camera.hpp"

namespace gps {
    //Camera constructor
    Camera::Camera(glm::vec3 cameraPosition, glm::vec3 cameraTarget, glm::vec3 cameraUp) : yaw(90.0f), pitch(0.0f), firstMouse(true), lastX(400.0f), lastY(300.0f) {
        this->cameraPosition = cameraPosition;
        this->cameraTarget = cameraTarget;
        this->cameraUpDirection = cameraUp;
        this->cameraFrontDirection = glm::normalize(cameraTarget - cameraPosition);
    }

    glm::mat4 Camera::getViewMatrix() {
        return glm::lookAt(cameraPosition, cameraTarget, cameraUpDirection);
    }

    glm::vec3 Camera::getCameraPos() {
        return cameraPosition;
    }

    glm::vec3 Camera::getCameraTarget()
    {
        return cameraTarget;
    }

    void Camera::setCameraPosition(const glm::vec3& position) {
    this->cameraPosition = position;
    }

    void Camera::setCameraTarget(const glm::vec3& target) {
        this->cameraTarget = target;
    }

    float Camera::zoom(double xoffset, double yoffset, float fov)
    {
        fov -= (float)yoffset;
        if (fov < 1.0f)
            fov = 1.0f;
        if (fov > 45.0f)
            fov = 45.0f;

        return fov;
    }


    void Camera::move(MOVE_DIRECTION direction, float speed) {
        if (direction == MOVE_BACKWARD)
        {
            cameraPosition -= cameraFrontDirection * speed;
        }
        else if (direction == MOVE_FORWARD)
        {
            cameraPosition += cameraFrontDirection * speed;
        }
        else if (direction == MOVE_LEFT)
        {
            cameraPosition -= glm::normalize(glm::cross(cameraFrontDirection, cameraUpDirection)) * speed;
        }
        else if (direction == MOVE_RIGHT)
        {
            cameraPosition += glm::normalize(glm::cross(cameraFrontDirection, cameraUpDirection)) * speed;
        }
        else if (direction == MOVE_UP) {
            cameraPosition += cameraUpDirection * speed;
        }
        else if (direction == MOVE_DOWN) {
            cameraPosition -= cameraUpDirection * speed;
        }
        cameraTarget = cameraPosition + cameraFrontDirection;
    }

    void Camera::rotate(float pitch, float yaw) {
        glm::vec3 direction;
        direction.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
        direction.y = sin(glm::radians(pitch));
        direction.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));

        cameraFrontDirection = glm::normalize(direction);
        cameraTarget = cameraPosition + cameraFrontDirection;
    }

    void Camera::processMouseMovement(double xpos, double ypos) {
        if (firstMouse) {
            lastX = xpos;
            lastY = ypos;
            firstMouse = false;
        }

        double xoffset = xpos - lastX;
        double yoffset = lastY - ypos; 
        lastX = xpos;
        lastY = ypos;

        double sensitivity = 0.1f; 
        xoffset *= sensitivity;
        yoffset *= sensitivity;

        yaw += xoffset;
        pitch += yoffset;

        if (pitch > 89.0f) pitch = 89.0f;
        if (pitch < -89.0f) pitch = -89.0f;

        rotate(pitch, yaw);
    }
}