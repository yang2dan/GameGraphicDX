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
	pEntityMaterial = NULL;
}

GameEntity::GameEntity( Mesh* pMesh = NULL, Material* pMaterial = NULL)
{
	//Initialize
	Position = XMFLOAT3(0, 0, 0);
	Rotation = XMFLOAT3(0, 0, 0);
	Scale = XMFLOAT3(1.0f, 1.0f, 1.0f);
	
	XMMATRIX W = XMMatrixIdentity();
	XMStoreFloat4x4(&worldMatrix, XMMatrixTranspose(W));

	pEntityMesh = pMesh;
	pEntityMaterial = pMaterial;
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

void GameEntity::setMaterial(Material* pMaterial)
{
	pEntityMaterial = pMaterial;
}

#if 0
void GameEntity::setVertexShader(SimpleVertexShader* pVS)
{
	vertexShader = pVS;
}
void GameEntity::setPixelShader(SimplePixelShader* pPS)
{
	pixelShader = pPS;
}
#endif
void GameEntity::DrawEntity(XMFLOAT4X4 viewMatrix, XMFLOAT4X4 projectionMatrix)
{
	XMMATRIX trans = XMMatrixTranslation(Position.x, Position.y, Position.z);
	//the order of rotation matters but here we assume a order ourselves
	XMMATRIX rotx  = XMMatrixRotationX(Rotation.x);
	XMMATRIX roty = XMMatrixRotationY(Rotation.y);
	XMMATRIX rotz = XMMatrixRotationZ(Rotation.z);
	XMMATRIX scale = XMMatrixScaling(Scale.x, Scale.y, Scale.z);
	XMMATRIX W = scale * rotz * roty * rotx * trans;
	XMStoreFloat4x4(&worldMatrix, XMMatrixTranspose(W)); // Transpose for HLSL!

	pEntityMaterial->GetVertexShader()->SetMatrix4x4("world", worldMatrix);
	pEntityMaterial->GetVertexShader()->SetMatrix4x4("view", viewMatrix);
	pEntityMaterial->GetVertexShader()->SetMatrix4x4("projection", projectionMatrix);

	pEntityMaterial->GetPixelShader()->SetShaderResourceView("diffuseTexture", pEntityMaterial->shaderResourceView);
	pEntityMaterial->GetPixelShader()->SetSamplerState("trilinear", pEntityMaterial->samplerState);

	pEntityMaterial->GetVertexShader()->SetShader(true);
	pEntityMaterial->GetPixelShader()->SetShader(true);

	pEntityMesh->DrawMesh();
}

GameEntity::~GameEntity()
{
}
