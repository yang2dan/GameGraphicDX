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

void Camera::SetVertexShader(SimpleVertexShader* VS)
{
	vertexShader = VS;
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

void Camera::SetCameraToVertexShader()
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

	//Setting both matrixes to vertex shader
	vertexShader->SetMatrix4x4("view", viewMatrix);
	vertexShader->SetMatrix4x4("projection", projectionMatrix);
}

void Camera::UpdateCameraDir(float XPitchMouseY, float YYawMouseX)
{
	//apply the rotation to camera direction and save the new direction
	XMVECTOR newCameraDir = XMVector3Rotate(XMLoadFloat3(&cameraLookToDir), //camera direction
											XMQuaternionRotationRollPitchYaw(XPitchMouseY, YYawMouseX, 0)//rotation
							);
	XMStoreFloat3(&cameraLookToDir, newCameraDir);

	//apply the rotation to camera up vector and save the new direction
	XMVECTOR newUpVector = XMVector3Rotate(XMLoadFloat3(&cameraUp), //camera up vector
		XMQuaternionRotationRollPitchYaw(XPitchMouseY, YYawMouseX, 0)//rotation
		);
	XMStoreFloat3(&cameraUp, newUpVector);
}

void Camera::MoveForward()
{
	XMVECTOR newCameraPos = XMLoadFloat3(&cameraPos);
	XMVECTOR CameraVelocity = XMLoadFloat3(&cameraLookToDir)/4;
	newCameraPos = newCameraPos + CameraVelocity;
	XMStoreFloat3(&cameraPos, newCameraPos);
}

void Camera::MoveBackward()
{
	XMVECTOR newCameraPos = XMLoadFloat3(&cameraPos);
	XMVECTOR CameraVelocity = (-1) * XMLoadFloat3(&cameraLookToDir)/4;
	newCameraPos = newCameraPos + CameraVelocity;
	XMStoreFloat3(&cameraPos, newCameraPos);
}

void Camera::MoveLeft()
{
	XMVECTOR newCameraPos = XMLoadFloat3(&cameraPos);
	XMVECTOR cameradir	  = XMLoadFloat3(&cameraLookToDir);
	XMVECTOR cameraup	  = XMLoadFloat3(&cameraUp);
	XMVECTOR CameraVelocity = XMVector3Cross(cameradir, cameraup)/4;
	newCameraPos = newCameraPos + CameraVelocity;
	XMStoreFloat3(&cameraPos, newCameraPos);
}

void Camera::MoveRight()
{
	XMVECTOR newCameraPos = XMLoadFloat3(&cameraPos);
	XMVECTOR cameradir = XMLoadFloat3(&cameraLookToDir);
	XMVECTOR cameraup = XMLoadFloat3(&cameraUp);
	XMVECTOR CameraVelocity = XMVector3Cross(cameradir, cameraup) / (-4);
	newCameraPos = newCameraPos + CameraVelocity;
	XMStoreFloat3(&cameraPos, newCameraPos);
}

void Camera::MoveUp()
{
	XMVECTOR newCameraPos = XMLoadFloat3(&cameraPos);
	XMVECTOR CameraVelocity = { 0, 0.25, 0, 0 };
	newCameraPos = newCameraPos + CameraVelocity;
	XMStoreFloat3(&cameraPos, newCameraPos);
}

void Camera::MoveDown()
{
	XMVECTOR newCameraPos = XMLoadFloat3(&cameraPos);
	XMVECTOR CameraVelocity = { 0, -0.25, 0, 0 };
	newCameraPos = newCameraPos + CameraVelocity;
	XMStoreFloat3(&cameraPos, newCameraPos);
}