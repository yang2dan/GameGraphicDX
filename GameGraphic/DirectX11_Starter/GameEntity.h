#pragma once
#include<DirectXMath.h>
#include"Mesh.h"
#include"SimpleShader.h"

//for the DX Math library
using namespace DirectX;

class GameEntity
{
public:
	//default constructor and destructor
	GameEntity();
	~GameEntity();

	//constructor accept mesh pointer
	GameEntity(Mesh* pMesh, SimpleVertexShader* pVS, SimplePixelShader* pPS);
	
	//setting transformations
	void setPositionX(float x);
	void setPositionY(float y);
	void setPositionZ(float z);

	void setRotationX(float x);
	void setRotationY(float y);
	void setRotationZ(float z);

	void setScaleX(float x);
	void setScaleY(float y);
	void setScaleZ(float z);

	//setting pointers
	void setMesh(Mesh* pMesh);
	void setVertexShader(SimpleVertexShader* pVS);
	void setPixelShader(SimplePixelShader* pPS);

	//Draw Entity
	void DrawEntity();

private:
	//Mesh pointer
	Mesh* pEntityMesh;

	//Transformations
	XMFLOAT3 Position;
	XMFLOAT3 Rotation;
	XMFLOAT3 Scale;

	//worldMatrix generated by 3 vectors above
	XMFLOAT4X4 worldMatrix;
	
	//Shader pointer
	SimpleVertexShader* vertexShader;
	SimplePixelShader* pixelShader;
};
