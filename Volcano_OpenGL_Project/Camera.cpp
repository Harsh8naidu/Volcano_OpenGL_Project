#include "Camera.h"

Camera::Camera() {
	m_Position = glm::vec3(0.f, 0.f, 0.f);
	m_Yaw = 0.f;
	m_Pitch = 0.f;
}

Camera::~Camera() {

}

glm::vec3 Camera::GetPosition() {
	return m_Position;
}

glm::vec3 Camera::GetForward() {
	glm::vec4 forward = glm::inverse(GetRotationMatrix()) * glm::vec4(0.f, 0.f, -1.f, 1.f); // Inverse rotation matrix * forward vector
	return glm::normalize(glm::vec3(forward));
}

glm::vec3 Camera::GetUp() {
	glm::vec4 up = glm::inverse(GetRotationMatrix()) * glm::vec4(0.f, 1.f, 0.f, 1.f); // Inverse rotation matrix * up vector
	return glm::vec3(up);
}

glm::vec3 Camera::GetRight() {
	glm::vec4 right = glm::inverse(GetRotationMatrix()) * glm::vec4(1.f, 0.f, 0.f, 1.f); // Inverse rotation matrix * right vector
	return glm::vec3(right);
}

glm::mat4 Camera::GetViewMatrix() {
	return GetRotationMatrix() * glm::translate(glm::mat4(1.f), -m_Position);
}

glm::mat4 Camera::GetRotationMatrix() {
	glm::mat4 rotation(1.f);
	rotation = glm::rotate(rotation, glm::radians(m_Pitch), glm::vec3(1.f, 0.f, 0.f));
	rotation = glm::rotate(rotation, glm::radians(m_Yaw), glm::vec3(0.f, 1.f, 0.f));
	return rotation;
}

void Camera::SetPosition(float x, float y, float z) {
	m_Position = glm::vec3(x, y, z);
}

void Camera::SetRotation(float x, float y, float z) {
	m_Yaw = x;
	m_Pitch = y;
}

void Camera::Move(glm::vec3 direction, float amount) {
	m_Position += direction * amount;
}

void Camera::Yaw(float angle) {
	m_Yaw += angle;
	NormalizeYaw();
}

void Camera::Pitch(float angle) {
	m_Pitch += angle;
	NormalizePitch();
}

void Camera::TurnTo(glm::vec3 position) {
	if (position == m_Position) {
		return;
	}

	glm::vec3 direction = glm::normalize(position - m_Position);
	m_Pitch = glm::degrees(asinf(-direction.y));
	NormalizePitch();

	m_Yaw = glm::degrees(atan2f(-direction.x, -direction.z));
	NormalizeYaw();
}

void Camera::MoveTo(glm::vec3 position) {
	m_Position = position;
}

void Camera::NormalizeYaw() {
	m_Yaw = fmodf(m_Yaw, 360.f);
	if (m_Yaw < 0.f) {
		m_Yaw += 360.f;
	}
}

void Camera::NormalizePitch() {
	if (m_Pitch > 85.f) {
		m_Pitch = 85.f;
	}
	else if (m_Pitch < -85.f) {
		m_Pitch = -85.f;
	}
}