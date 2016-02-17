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

	void SetCameraToVertexShader();
	void SetVertexShader(SimpleVertexShader* VS);
	void SetAspectRatio(float asr);
	void SetFieldofViewAngle(float angle);
	void SetNearClipDis(float neardis);
	void SetFarClipDis(float fardis);
	void UpdateCameraDir(float XPitchMouseY, float YYawMouseX);
	void MoveForward();
	void MoveBackward();
	void MoveLeft();
	void MoveRight();
	void MoveUp();
	void MoveDown();



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
	
	//Vertex Shader
	SimpleVertexShader* vertexShader;
};

