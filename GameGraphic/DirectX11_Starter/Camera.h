#pragma once
#include<DirectXMath.h>
#include "SimpleShader.h"
//for the DX Math library
using namespace DirectX;

class Camera
{
public:
	Camera();
	~Camera();

	void SetAspectRatio(float asr);
	void SetFieldofViewAngle(float angle);
	void SetNearClipDis(float neardis);
	void SetFarClipDis(float fardis);
	
	void UpdateCameraDir(float XPitchMouseY, float YYawMouseX);
	
	void UpdateVPMatrixes();
	
	XMFLOAT4X4 GetViewMatrix();
	XMFLOAT4X4 GetProjectionMatrix();
	
	void MoveForward(float fSpeed);
	void MoveBackward(float fSpeed);
	void MoveLeft(float fSpeed);
	void MoveRight(float fSpeed);
	void MoveUp(float fSpeed);
	void MoveDown(float fSpeed);
	
	XMFLOAT3 GetCameraPosition();



private:
	//view matrix
	XMFLOAT3	cameraPos;
	XMFLOAT3	cameraLookToDir;
	XMFLOAT3	cameraUp;
	XMFLOAT4X4	viewMatrix;

	//Projection matrix
	float		viewAngleField;
	float		aspectRatio;
	float		nearClipDistance;
	float		farClipDistance;
	XMFLOAT4X4	projectionMatrix;
};

