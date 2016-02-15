#include "GameEntity.h"



GameEntity::GameEntity()
{
	//Initialize
	Position = XMFLOAT3(0, 0, 0);
	Rotation = XMFLOAT3(0, 0, 0);
	Scale	 = XMFLOAT3(1.0f, 1.0f, 1.0f);

	XMMATRIX W = XMMatrixIdentity();
	XMStoreFloat4x4(&worldMatrix, XMMatrixTranspose(W));

	pEntityMesh = NULL;
	vertexShader = NULL;
	pixelShader = NULL;
}

GameEntity::GameEntity( Mesh* pMesh =NULL,
						SimpleVertexShader* pVS = NULL,
						SimplePixelShader* pPS = NULL)
{
	//Initialize
	Position = XMFLOAT3(0, 0, 0);
	Rotation = XMFLOAT3(0, 0, 0);
	Scale = XMFLOAT3(1.0f, 1.0f, 1.0f);
	
	XMMATRIX W = XMMatrixIdentity();
	XMStoreFloat4x4(&worldMatrix, XMMatrixTranspose(W));

	pEntityMesh = pMesh;
	vertexShader = pVS;
	pixelShader = pPS;
}

void GameEntity::setPositionX(float x)
{
	Position.x = x;
}
void GameEntity::setPositionY(float y)
{
	Position.y = y;
}
void GameEntity::setPositionZ(float z)
{
	Position.z = z;
}
void GameEntity::setRotationX(float x)
{
	Rotation.x = x;
}
void GameEntity::setRotationY(float y)
{
	Rotation.y = y;
}
void GameEntity::setRotationZ(float z)
{
	Rotation.z = z;
}
void GameEntity::setScaleX(float x)
{
	Scale.x = x;
}
void GameEntity::setScaleY(float y)
{
	Scale.y = y;
}
void GameEntity::setScaleZ(float z)
{
	Scale.z = z;
}

void GameEntity::setMesh(Mesh* pMesh)
{
	pEntityMesh = pMesh;
}
void GameEntity::setVertexShader(SimpleVertexShader* pVS)
{
	vertexShader = pVS;
}
void GameEntity::setPixelShader(SimplePixelShader* pPS)
{
	pixelShader = pPS;
}

void GameEntity::DrawEntity()
{
	XMMATRIX trans = XMMatrixTranslation(Position.x, Position.y, Position.z);
	//the order of rotation matters but here we assume a order ourselves
	XMMATRIX rotx  = XMMatrixRotationX(Rotation.x);
	XMMATRIX roty = XMMatrixRotationY(Rotation.y);
	XMMATRIX rotz = XMMatrixRotationZ(Rotation.z);
	XMMATRIX scale = XMMatrixScaling(Scale.x, Scale.y, Scale.z);
	XMMATRIX W = scale * rotz * roty * rotx * trans;
	XMStoreFloat4x4(&worldMatrix, XMMatrixTranspose(W)); // Transpose for HLSL!

	vertexShader->SetMatrix4x4("world", worldMatrix);
	vertexShader->SetShader(true);
	pixelShader->SetShader(true);
	pEntityMesh->DrawMesh();
}

GameEntity::~GameEntity()
{
}
