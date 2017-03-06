#pragma once

#include <d3d11.h>
#include "Vertex.h"
#include "DirectXGameCore.h"

class Mesh
{
public:
	Mesh();
	~Mesh();
	Mesh(Vertex* _verticies, int vertexNumber, int* _indices, int indNumber);
	Mesh(char* objFileName);
	void LoadObjFile(char* objFileName);
	void setVerticies(Vertex* _verticies, int number);
	void setIndices(int* _indices, int number);
	void CreateBuffer();
	void DrawMesh();
	void SetD3DDevice(ID3D11Device* _device);
	void SetD3DDevContext(ID3D11DeviceContext* _devContext);

	void CalculateTangents(Vertex* verts, int numVerts, unsigned int* indices, int numIndices);


private:
	Vertex*					pVerticies;
	int*					pIndices;
	ID3D11Device*           device;
	ID3D11DeviceContext*    deviceContext;
	ID3D11Buffer*			vertexBuffer;
	ID3D11Buffer*			indexBuffer;
	int						VertexNumber;
	int						IndicesNumber;

	int temp = 0;

};

