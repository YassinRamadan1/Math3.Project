#pragma once

#include <glad/glad.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

enum Camera_Movement {
	FORWARD, BACKWARD, LEFT, RIGHT
};

const float YAW = -90.0f;
const float PITCH = 0.0f;
const float SENSITIVITY = 0.05f;
const float SPEED = 2.5f;
const float ZOOM = 45.0f;

class Camera {

public:
	glm::vec3 m_Position;
	glm::vec3 m_Front;
	glm::vec3 m_Up;
	glm::vec3 m_Right;
	glm::vec3 m_WorldUp;

	float m_Speed;
	float m_Sensitivity;
	float m_Zoom;
	float m_Yaw;
	float m_Pitch;
	Camera(glm::vec3 position = glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f), float yaw = YAW, float pitch = PITCH) : m_Front(glm::vec3(0.0f,0.0f,-1.0f)), m_Speed(SPEED), m_Sensitivity(SENSITIVITY), m_Zoom(ZOOM) {
		m_Position = position;
		m_WorldUp = up;
		m_Yaw = yaw;
		m_Pitch = pitch;
		UpdateCameraVectors();
	}


	void processKeyboard(Camera_Movement direction, float dt ) {

		float velocity = m_Speed * dt;

		if (direction == FORWARD)
			m_Position += velocity * m_Front;
		if (direction == BACKWARD)
			m_Position -= velocity * m_Front;
		if (direction == LEFT)
			m_Position -= velocity * m_Right;
		if (direction == RIGHT)
			m_Position += velocity * m_Right;

	}

	void processMouseMovement(float xOffset, float yOffset) {

		xOffset *= m_Sensitivity;
		yOffset *= m_Sensitivity;
		m_Yaw += xOffset;
		m_Pitch += yOffset;
		m_Pitch = m_Pitch > 89.0f ? 89.0f : m_Pitch < -89.0f ? -89.0f : m_Pitch;
		UpdateCameraVectors();
	}

	void processMouseScroll(float yOffset) {

		m_Zoom -= float(yOffset);
		m_Zoom = m_Zoom > 45.0f ? 45.0f : m_Zoom < 1.0 ? 1.0 : m_Zoom;
	}


	glm::mat4 getViewMatrix() {
		return glm::lookAt(m_Position, m_Position + m_Front, m_WorldUp); //MylookAt(m_Position, m_Position + m_Front, m_WorldUp);
	}


private:

	void UpdateCameraVectors() {

		m_Front.x = cos(glm::radians(m_Pitch)) * cos(glm::radians(m_Yaw));
		m_Front.y = sin(glm::radians(m_Pitch));
		m_Front.z = cos(glm::radians(m_Pitch)) * sin(glm::radians(m_Yaw));
		m_Front = glm::normalize(m_Front);
		m_Right = glm::normalize(glm::cross(m_Front, m_WorldUp));
		m_Up = glm::cross(m_Right, m_Front);
	}

	glm::mat4 MylookAt(glm::vec3 eye, glm::vec3 target, glm::vec3 up) {

		glm::vec3 f = glm::normalize(eye - target);
		glm::vec3 r = glm::normalize(glm::cross(up, f));
		glm::vec3 u = glm::cross(f, r);
		glm::mat4 view = glm::transpose(glm::mat4(glm::vec4(r, 0.0f), glm::vec4(u, 0.0f), glm::vec4(f, 0.0f), glm::vec4(0.0f, 0.0f, 0.0f, 1.0f)));
		view = glm::translate(view, -eye);
		return view;
	}
};
