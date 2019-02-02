#include "Camera.hpp"

#include <algorithm>
#include <cmath>

namespace VkRenderer
{

void Camera::Reset()
{
	m_Target = glm::vec3(0.0f, 0.0f, 0.0f);

	m_Yaw = glm::radians(0.0f);
	m_Pitch = glm::radians(0.0f);
	m_Radius = 3.0f;

	m_NearZ = 0.1f;
	m_FarZ = 100.0f;

	m_Resolution = glm::vec2(800.0f, 600.0f);;
	m_Fov = glm::vec2(glm::radians(45.0f), glm::radians(45.0f));
}

void Camera::UpdateYaw(float Delta)
{
	m_Yaw += (Delta * m_YawSpeed);
	ClampYaw(m_Yaw);
}

void Camera::UpdatePitch(float Delta)
{
	m_Pitch += (Delta * m_PitchSpeed);
	ClampPitch(m_Pitch);
}

void Camera::UpdateRadius(float Delta)
{
	m_Radius += (Delta * m_RadiusSpeed * m_Radius / 3.0f * 7.5f);
	ClampRadius(m_Radius);
}

void Camera::UpdateTarget(float DeltaX, float DeltaY)
{
	float X = cos(m_Yaw) * cos(m_Pitch);
	float Y = sin(m_Yaw) * cos(m_Pitch);
	float Z = sin(m_Pitch);

	glm::vec3 Up(0.0f, 0.0f, 1.0f);
	glm::vec3 View(-X, -Y, -Z);
	glm::vec3 Horizontal = glm::normalize(glm::cross(View, Up));
	Up = glm::normalize(glm::cross(Horizontal, View));
	m_Target += ((Up * DeltaY + Horizontal * DeltaX) * m_TargetSpeed * m_Radius / 3.0f);
}

void Camera::SetNearFarZ(float NearZ, float FarZ)
{
	assert(NearZ > 0.0f && FarZ > NearZ);
	m_NearZ = NearZ;
	m_FarZ = FarZ;
}

void Camera::SetFov(float FovX)
{
	assert(FovX > 0.0f);
	m_Fov.x = FovX;
	m_Fov.y = atan(tan(FovX * 0.5f) * (m_Resolution.y / m_Resolution.x)) * 2.0f;
}

void Camera::SetResolution(float Width, float Height)
{
	assert(Width > 0.0f && Height > 0.0f);
	m_Resolution.x = Width;
	m_Resolution.y = Height;
}

void Camera::RetriveData(glm::vec3 & Target, glm::vec3 & Eye, glm::vec3 & Up, glm::vec2 & Fov, float & NearZ, float & FarZ)
{
	float X = cos(m_Yaw) * cos(m_Pitch);
	float Y = sin(m_Yaw) * cos(m_Pitch);
	float Z = sin(m_Pitch);
	glm::vec3 ToCamera(X, Y, Z);

	Target = m_Target;
	Eye = m_Target + ToCamera * m_Radius;
	Up = glm::vec3(0.0f, 0.0f, 1.0f);

	Fov = m_Fov;

	NearZ = m_NearZ;
	FarZ = m_FarZ;
}

void Camera::ClampYaw(float & Yaw) const
{
	Yaw = std::fmod(Yaw, glm::radians(360.0f));
}

void Camera::ClampPitch(float & Pitch) const
{
	Pitch = std::clamp(Pitch, glm::radians(-89.9f), glm::radians(89.9f));
}

void Camera::ClampRadius(float & Radius) const
{
	Radius = std::clamp(Radius, 1e-4f, 50.0f);
}

} // End namespace