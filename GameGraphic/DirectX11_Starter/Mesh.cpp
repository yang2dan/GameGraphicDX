#include "Mesh.h"

Mesh::Mesh()
{
	pVerticies		= NULL;
	pIndices		= NULL;
	device			= NULL;
	deviceContext	= NULL;
	vertexBuffer	= NULL;
	indexBuffer		= NULL;
	VertexNumber	= 0;
	IndicesNumber	= 0;
}

Mesh::Mesh(Vertex* _verticies, int vertexNumber, int* _indices, int indNumber)
{
	setVerticies(_verticies, vertexNumber);
	setIndices(_indices, indNumber);
	CreateBuffer();
}

Mesh::~Mesh()
{
	//free the vertex and index array which were created by Mesh class
	free(pVerticies);
	free(pIndices);
	//set device and deviceContext to NULL, dont release them here
	device = NULL;
	deviceContext = NULL;
	//release the buffer created by Mesh class
	ReleaseMacro(vertexBuffer);
	ReleaseMacro(indexBuffer);
}

void Mesh::setVerticies(Vertex* _verticies, int number)
{
	//free old space first before store a new array
	free(pVerticies);
	//create new space and save the new vertex data
	pVerticies = (Vertex*)malloc(sizeof(Vertex) * number);
	memset(pVerticies, 0, sizeof(Vertex) * number);
	memcpy(pVerticies, _verticies, sizeof(Vertex) * number);
	//save the number of Vertex
	VertexNumber = number;
}

void Mesh::setIndices(int* _indices, int number)
{
	free(pIndices);
	pIndices = (int*)malloc(sizeof(int) * number);
	memset(pIndices, 0, sizeof(int) * number);
	memcpy(pIndices, _indices, sizeof(int) * number);
	IndicesNumber = number;
}

void Mesh::CreateBuffer()
{
	// Create the VERTEX BUFFER description -----------------------------------
	D3D11_BUFFER_DESC vbd;
	vbd.Usage = D3D11_USAGE_IMMUTABLE;
	vbd.ByteWidth = sizeof(Vertex) * VertexNumber;       // number of vertices in the buffer
	vbd.BindFlags = D3D11_BIND_VERTEX_BUFFER; // Tells DirectX this is a vertex buffer
	vbd.CPUAccessFlags = 0;
	vbd.MiscFlags = 0;
	vbd.StructureByteStride = 0;

	// Create the proper struct to hold the initial vertex data
	// - This is how we put the initial data into the buffer
	D3D11_SUBRESOURCE_DATA initialVertexData;
	initialVertexData.pSysMem = pVerticies;

	// Actually create the buffer with the initial data
	// - Once we do this, we'll NEVER CHANGE THE BUFFER AGAIN
	HR(device->CreateBuffer(&vbd, &initialVertexData, &vertexBuffer));


	// Create the INDEX BUFFER description ------------------------------------
	// - The description is created on the stack because we only need
	//    it to create the buffer.  The description is then useless.
	D3D11_BUFFER_DESC ibd;
	ibd.Usage = D3D11_USAGE_IMMUTABLE;
	ibd.ByteWidth = sizeof(int) * IndicesNumber;         // number of indices in the buffer
	ibd.BindFlags = D3D11_BIND_INDEX_BUFFER; // Tells DirectX this is an index buffer
	ibd.CPUAccessFlags = 0;
	ibd.MiscFlags = 0;
	ibd.StructureByteStride = 0;

	// Create the proper struct to hold the initial index data
	// - This is how we put the initial data into the buffer
	D3D11_SUBRESOURCE_DATA initialIndexData;
	initialIndexData.pSysMem = pIndices;

	// Actually create the buffer with the initial data
	// - Once we do this, we'll NEVER CHANGE THE BUFFER AGAIN
	HR(device->CreateBuffer(&ibd, &initialIndexData, &indexBuffer));
}

void Mesh::DrawMesh()
{
	// Set buffers in the input assembler
	//  - Do this ONCE PER OBJECT you're drawing, since each object might
	//    have different geometry.
	UINT stride = sizeof(Vertex);
	UINT offset = 0;
	deviceContext->IASetVertexBuffers(0, 1, &vertexBuffer, &stride, &offset);
	deviceContext->IASetIndexBuffer(indexBuffer, DXGI_FORMAT_R32_UINT, 0);

	// Finally do the actual drawing
	//  - Do this ONCE PER OBJECT you intend to draw
	//  - This will use all of the currently set DirectX "stuff" (shaders, buffers, etc)
	//  - DrawIndexed() uses the currently set INDEX BUFFER to look up corresponding
	//     vertices in the currently set VERTEX BUFFER
	deviceContext->DrawIndexed(
		IndicesNumber,     // The number of indices to use (we could draw a subset if we wanted)
		0,     // Offset to the first index we want to use
		0);    // Offset to add to each index when looking up vertices
}

void Mesh::SetD3DDevice(ID3D11Device* _device)
{
	device = _device;
}

void Mesh::SetD3DDevContext(ID3D11DeviceContext* _devContext)
{
	deviceContext = _devContext;
}
