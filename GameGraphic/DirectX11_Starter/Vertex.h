#pragma once

#include <DirectXMath.h>

using namespace DirectX;

// --------------------------------------------------------
// A custom vertex definition - You will eventually ADD TO this!
// --------------------------------------------------------
struct Vertex
{
	XMFLOAT3 Position;	    // The position of the vertex
	XMFLOAT3 Normal;		//normal vector for lighting
	XMFLOAT3 Tangent;		//Tangent for normal mapping
	XMFLOAT2 UV;			//UV coordinate for texture mapping

	//DirectX::XMFLOAT4 Color;        // The color of the vertex
};