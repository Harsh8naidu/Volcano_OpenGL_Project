#pragma once
#include <glm/fwd.hpp>
#include <glm/ext.hpp>

class Camera
{
public:
	Camera();
	~Camera();

	void Update();
	void SetPosition(float x, float y, float z);
	void SetRotation(float x, float y, float z);

	glm::vec3 GetPosition();
	glm::vec3 GetForward();
	glm::vec3 GetUp();
	glm::vec3 GetRight();
	glm::mat4 GetViewMatrix();
	glm::mat4 GetRotationMatrix();
	
	void Move(glm::vec3 direction, float amount);
	void Yaw(float angle);
	void Pitch(float angle);

	void TurnTo(glm::vec3 position);
	void MoveTo(glm::vec3 position);

private:
	void NormalizeYaw();
	void NormalizePitch();

	glm::vec3 m_Position;
	float m_Yaw;
	float m_Pitch;
};
