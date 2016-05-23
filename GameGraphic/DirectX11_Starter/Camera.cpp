#include "Camera.h"



Camera::Camera()
{
	//camera default setting
	//view matrix
	cameraPos			= XMFLOAT3(0.0f, 0.0f, -10.0f);
	cameraLookToDir		= XMFLOAT3(0, 0, 1);
	cameraUp			= XMFLOAT3(0, 1, 0);
	//Projection matrix
	viewAngleField		= 0.25f * 3.1415926535f;
	nearClipDistance	= 0.1f;
	farClipDistance		= 100.0f;
}

Camera::~Camera()
{
}

void Camera::SetAspectRatio(float asr)
{
	aspectRatio = asr;
}

void Camera::SetFieldofViewAngle(float angle)
{
	viewAngleField = angle;
}

void Camera::SetNearClipDis(float neardis)
{
	nearClipDistance = neardis;
}

void Camera::SetFarClipDis(float fardis)
{
	farClipDistance = fardis;
}

void Camera::UpdateVPMatrixes()
{
	//view matrix
	XMVECTOR pos = XMLoadFloat3(&cameraPos);
	XMVECTOR dir = XMLoadFloat3(&cameraLookToDir);
	XMVECTOR up	 = XMLoadFloat3(&cameraUp);
	XMMATRIX V = XMMatrixLookToLH(
		pos,     // The position of the "camera"
		dir,     // Direction the camera is looking
		up);     // "Up" direction in 3D space (prevents roll)
	XMStoreFloat4x4(&viewMatrix, XMMatrixTranspose(V)); // Transpose for HLSL!

	//projection matrix
	XMMATRIX P = XMMatrixPerspectiveFovLH(
		viewAngleField,
		aspectRatio,
		nearClipDistance,
		farClipDistance);
	XMStoreFloat4x4(&projectionMatrix, XMMatrixTranspose(P));

#if 0
	//Setting both matrixes to vertex shader
	vertexShader->SetMatrix4x4("view", viewMatrix);
	vertexShader->SetMatrix4x4("projection", projectionMatrix);
#endif
}

void Camera::UpdateCameraDir(float XPitchMouseY, float YYawMouseX)
{
	//apply the rotation to camera direction and save the new direction
	XMVECTOR cameradir = XMLoadFloat3(&cameraLookToDir);
	XMVECTOR cameraup = XMLoadFloat3(&cameraUp);
	XMVECTOR newCameraDir = XMLoadFloat3(&cameraLookToDir);
	XMMATRIX PitchMatrix = XMMatrixRotationAxis(-XMVector3Cross(cameradir, cameraup), XPitchMouseY);
	XMMATRIX YawMatrix = XMMatrixRotationAxis(cameraup, YYawMouseX);
	newCameraDir = XMVector3Transform(newCameraDir, PitchMatrix);
	newCameraDir = XMVector3Transform(newCameraDir, YawMatrix);
	XMStoreFloat3(&cameraLookToDir, newCameraDir);

	//XMVECTOR newUpVector = XMLoadFloat3(&cameraUp);
	//newUpVector = XMVector3Transform(newUpVector, PitchMatrix);
	//XMStoreFloat3(&cameraUp, newUpVector);
}

void Camera::MoveForward(float fSpeed)
{
	XMVECTOR newCameraPos = XMLoadFloat3(&cameraPos);
	XMVECTOR CameraVelocity = XMLoadFloat3(&cameraLookToDir) * fSpeed;
	newCameraPos = newCameraPos + CameraVelocity;
	XMStoreFloat3(&cameraPos, newCameraPos);
}

void Camera::MoveBackward(float fSpeed)
{
	XMVECTOR newCameraPos = XMLoadFloat3(&cameraPos);
	XMVECTOR CameraVelocity = (-1) * XMLoadFloat3(&cameraLookToDir) * fSpeed;
	newCameraPos = newCameraPos + CameraVelocity;
	XMStoreFloat3(&cameraPos, newCameraPos);
}

void Camera::MoveLeft(float fSpeed)
{
	XMVECTOR newCameraPos = XMLoadFloat3(&cameraPos);
	XMVECTOR cameradir	  = XMLoadFloat3(&cameraLookToDir);
	XMVECTOR cameraup	  = XMLoadFloat3(&cameraUp);
	XMVECTOR CameraVelocity = XMVector3Cross(cameradir, cameraup) * fSpeed;
	newCameraPos = newCameraPos + CameraVelocity;
	XMStoreFloat3(&cameraPos, newCameraPos);
}

void Camera::MoveRight(float fSpeed)
{
	XMVECTOR newCameraPos = XMLoadFloat3(&cameraPos);
	XMVECTOR cameradir = XMLoadFloat3(&cameraLookToDir);
	XMVECTOR cameraup = XMLoadFloat3(&cameraUp);
	XMVECTOR CameraVelocity = (-1) * XMVector3Cross(cameradir, cameraup) * fSpeed;
	newCameraPos = newCameraPos + CameraVelocity;
	XMStoreFloat3(&cameraPos, newCameraPos);
}

void Camera::MoveUp(float fSpeed)
{
	XMVECTOR newCameraPos = XMLoadFloat3(&cameraPos);
	XMVECTOR CameraVelocity = { 0, 1, 0, 0 };
	newCameraPos = newCameraPos + CameraVelocity * fSpeed;
	XMStoreFloat3(&cameraPos, newCameraPos);
}

void Camera::MoveDown(float fSpeed)
{
	XMVECTOR newCameraPos = XMLoadFloat3(&cameraPos);
	XMVECTOR CameraVelocity = { 0, -1, 0, 0 };
	newCameraPos = newCameraPos + CameraVelocity * fSpeed;
	XMStoreFloat3(&cameraPos, newCameraPos);
}

XMFLOAT4X4 Camera::GetViewMatrix()
{
	return viewMatrix;
}
XMFLOAT4X4 Camera::GetProjectionMatrix()
{
	return projectionMatrix;
}

XMFLOAT3 Camera::GetCameraPosition()
{
	return cameraPos;
}