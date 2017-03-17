// ----------------------------------------------------------------------------
//  A few notes on project settings
//
//  - The project is set to use the UNICODE character set
//    - This was changed in Project Properties > Config Properties > General > Character Set
//    - This basically adds a "#define UNICODE" to the project
//
//  - The include directories were automagically correct, since the DirectX 
//    headers and libs are part of the windows SDK
//    - For instance, $(WindowsSDK_IncludePath) is set as a project include 
//      path by default.  That's where the DirectX headers are located.
//
//  - Two libraries had to be manually added to the Linker Input Dependencies
//    - d3d11.lib
//    - d3dcompiler.lib
//    - This was changed in Project Properties > Config Properties > Linker > Input > Additional Dependencies
//
//  - The Working Directory was changed to match the actual .exe's 
//    output directory, since we need to load the compiled shader files at run time
//    - This was changed in Project Properties > Config Properties > Debugging > Working Directory
//
// ----------------------------------------------------------------------------

#include "MyDemoGame.h"
#include "Vertex.h"
#include "WICTextureLoader.h"
#include "DDSTextureLoader.h"

// For the DirectX Math library
using namespace DirectX;


#pragma region Win32 Entry Point (WinMain)
// --------------------------------------------------------
// Win32 Entry Point - Where your program starts
// --------------------------------------------------------
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE prevInstance,
				   PSTR cmdLine, int showCmd)
{
	// Enable run-time memory check for debug builds.
#if defined(DEBUG) | defined(_DEBUG)
	_CrtSetDbgFlag( _CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF );
#endif

	// Create the game object.

	MyDemoGame game(hInstance);
	
	// This is where we'll create the window, initialize DirectX, 
	// set up geometry and shaders, etc.
	if( !game.Init() )
		return 0;
	
	// All set to run the game loop
	return game.Run();
}

#pragma endregion

#pragma region Constructor / Destructor
// --------------------------------------------------------
// Base class constructor will set up all of the underlying
// fields, and then we can overwrite any that we'd like
// --------------------------------------------------------
MyDemoGame::MyDemoGame(HINSTANCE hInstance) 
	: DirectXGameCore(hInstance)
{
	// Set up a custom caption for the game window.
	// - The "L" before the string signifies a "wide character" string
	// - "Wide" characters take up more space in memory (hence the name)
	// - This allows for an extended character set (more fancy letters/symbols)
	// - Lots of Windows functions want "wide characters", so we use the "L"
	windowCaption = L"Chen's Engine";

	// Custom window size - will be created by Init() later
	windowWidth = 1280;
	windowHeight = 720;
}

// --------------------------------------------------------
// Cleans up our DirectX stuff and any objects we need to delete
// - When you make new DX resources, you need to release them here
// - If you don't, you get a lot of scary looking messages in Visual Studio
// --------------------------------------------------------
MyDemoGame::~MyDemoGame()
{
	// Release any D3D stuff that's still hanging out
	ReleaseMacro(vertexBuffer);
	ReleaseMacro(indexBuffer);

	// Delete our simple shaders
	delete vertexShader;
	delete pixelShader;
}

#pragma endregion

#pragma region Initialization

// --------------------------------------------------------
// Initializes the base class (including the window and D3D),
// sets up our geometry and loads the shaders (among other things)
// --------------------------------------------------------
bool MyDemoGame::Init()
{
	// Call the base class's Init() method to create the window,
	// initialize DirectX, etc.
	if( !DirectXGameCore::Init() )
		return false;

	// Helper methods to create something to draw, load shaders to draw it 
	// with and set up matrices so we can see how to pass data to the GPU.
	//  - For your own projects, feel free to expand/replace these.
	LoadShaders();
	
	CreateMaterial();

	//load mesh
	CreateGeometry();

	// Tell the input assembler stage of the pipeline what kind of
	// geometric primitives we'll be using and how to interpret them
	deviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	//Camera Initialize
	FPScamera.SetAspectRatio(aspectRatio);
	
	//Light Initialize
	//dirlight1.AmbientColor = XMFLOAT4(0.2, 0.2, 0.2, 1.0);
	//dirlight1.DiffuseColor = XMFLOAT4(0.3, 0.3, 0.3, 1.0);
	dirlight1.Direction = XMFLOAT3(1, -1, 1);

	pointlight1.Postion = XMFLOAT3(0, 5, -5);
	pointlight1.Color	 = XMFLOAT4(1, 1, 1, 1);

	// Create a description of the blend state I want
	D3D11_BLEND_DESC blendDesc = {};

	// Set up some of the basic options
	blendDesc.AlphaToCoverageEnable = false;
	blendDesc.IndependentBlendEnable = false;

	// Set up the blend options for the first render target
	blendDesc.RenderTarget[0].BlendEnable = true;

	// Settings for how colors (RGB) are blended (ALPHA BLENDING)
	blendDesc.RenderTarget[0].SrcBlend = D3D11_BLEND_SRC_ALPHA;
	blendDesc.RenderTarget[0].DestBlend = D3D11_BLEND_INV_SRC_ALPHA;
	blendDesc.RenderTarget[0].BlendOp = D3D11_BLEND_OP_ADD;

	// Settings for ADDITIVE BLENDING
	//blendDesc.RenderTarget[0].SrcBlend = D3D11_BLEND_ONE;
	//blendDesc.RenderTarget[0].DestBlend = D3D11_BLEND_ONE;
	//blendDesc.RenderTarget[0].BlendOp = D3D11_BLEND_OP_ADD;

	// Settings for how the alpha channel is blended
	blendDesc.RenderTarget[0].SrcBlendAlpha = D3D11_BLEND_ONE;
	blendDesc.RenderTarget[0].DestBlendAlpha = D3D11_BLEND_ZERO;
	blendDesc.RenderTarget[0].BlendOpAlpha = D3D11_BLEND_OP_ADD;

	// Write masks
	blendDesc.RenderTarget[0].RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;

	// Create the blend state object
	device->CreateBlendState(&blendDesc, &blendState);

	// Turn on the newly created blend state
	float factors[4] = { 1,1,1,1 };
	deviceContext->OMSetBlendState(
		blendState,
		factors,
		0xFFFFFFFF);
	// Successfully initialized
	return true;
}

// --------------------------------------------------------
// Create material from pictures using SDK method
// --------------------------------------------------------
void MyDemoGame::CreateMaterial()
{
	//Init Material
	//load texture
	CreateWICTextureFromFile(	device,
								deviceContext,
								L"ironman.bmp",
								0,
								&material1.texture);
	//load normalmap
	CreateWICTextureFromFile(	device, 
								deviceContext, 
								L"ironmannormal.bmp", 
								0, 
								&material1.normalMap);
	//load specTexture
	CreateWICTextureFromFile(	device, 
								deviceContext, 
								L"ironmanspec.bmp", 
								0, 
								&material1.specTexture);
	//load skybox texture
	CreateDDSTextureFromFile(	device,
								deviceContext,
								L"SunnyCubeMap.dds",
								0,
								&skyBoxMaterial.skyTexture);

	material1.skyTexture = skyBoxMaterial.skyTexture;
	
	//creat sampler state
	D3D11_SAMPLER_DESC samplerDesc = {};
	samplerDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
	samplerDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
	samplerDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
	samplerDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
	samplerDesc.MaxLOD = D3D11_FLOAT32_MAX;

	device->CreateSamplerState(&samplerDesc, &material1.samplerState);
	device->CreateSamplerState(&samplerDesc, &skyBoxMaterial.samplerState);

	//Create the rasterizer state for sky box
	D3D11_RASTERIZER_DESC rsDesc = {};
	rsDesc.FillMode = D3D11_FILL_SOLID;
	rsDesc.CullMode = D3D11_CULL_FRONT;
	rsDesc.DepthClipEnable = true;
	device->CreateRasterizerState(&rsDesc, &skyBoxMaterial.rsState);

	//Create the depth stencil for sky box
	D3D11_DEPTH_STENCIL_DESC dsDesc = {};
	dsDesc.DepthEnable = true;
	dsDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
	dsDesc.DepthFunc = D3D11_COMPARISON_LESS_EQUAL;
	device->CreateDepthStencilState(&dsDesc, &skyBoxMaterial.dsState);
	
	material1.SetVertexShader(vertexShader);
	material1.SetPixelShader(pixelShader);
	skyBoxMaterial.SetVertexShader(skyboxVertexShader);
	skyBoxMaterial.SetPixelShader(skyboxPixelShader);
}




// --------------------------------------------------------
// Loads shaders from compiled shader object (.cso) files
// - These simple shaders provide helpful methods for sending
//   data to individual variables on the GPU
// --------------------------------------------------------
void MyDemoGame::LoadShaders()
{
	vertexShader = new SimpleVertexShader(device, deviceContext);
	vertexShader->LoadShaderFile(L"VertexShader.cso");

	pixelShader = new SimplePixelShader(device, deviceContext);
	pixelShader->LoadShaderFile(L"PixelShader.cso");

	pixelShaderST = new SimplePixelShader(device, deviceContext);
	pixelShaderST->LoadShaderFile(L"PixelShaderSpecTexture.cso");

	//sky box shader
	skyboxVertexShader = new SimpleVertexShader(device, deviceContext);
	skyboxVertexShader->LoadShaderFile(L"SkyBoxVertexShader.cso");

	skyboxPixelShader = new SimplePixelShader(device, deviceContext);
	skyboxPixelShader->LoadShaderFile(L"SkyBoxPixelShader.cso");

	pixelShaderReflect = new SimplePixelShader(device, deviceContext);
	pixelShaderReflect->LoadShaderFile(L"PixelShaderReflection.cso");

}


// --------------------------------------------------------
// Creates the geometry we're going to draw - a single triangle for now
// --------------------------------------------------------
void MyDemoGame::CreateGeometry()
{
	//Load obj file
	CubeMesh.SetD3DDevice(GetDevice());
	CubeMesh.SetD3DDevContext(GetDevContext());
	CubeMesh.LoadObjFile("ironman.obj");

	SkyBoxMesh.SetD3DDevice(GetDevice());
	SkyBoxMesh.SetD3DDevContext(GetDevContext());
	SkyBoxMesh.LoadObjFile("cube.obj");


	//Set entities
	CubeEntity.setMesh(&CubeMesh);
	CubeEntity.setMaterial(&material1);
	SkyBoxEntity.setMesh(&SkyBoxMesh);
	SkyBoxEntity.setMaterial(&skyBoxMaterial);

}

#pragma endregion

#pragma region Window Resizing

// --------------------------------------------------------
// Handles resizing DirectX "stuff" to match the (usually) new
// window size and updating our projection matrix to match
// --------------------------------------------------------
void MyDemoGame::OnResize()
{
	// Handle base-level DX resize stuff
	DirectXGameCore::OnResize();
	//Change camera aspect ratio
	FPScamera.SetAspectRatio(aspectRatio);
#if 0
	// Update our projection matrix since the window size changed
	XMMATRIX P = XMMatrixPerspectiveFovLH(
		0.25f * 3.1415926535f,	// Field of View Angle
		aspectRatio,		  	// Aspect ratio
		0.1f,				  	// Near clip plane distance
		100.0f);			  	// Far clip plane distance
	XMStoreFloat4x4(&projectionMatrix, XMMatrixTranspose(P)); // Transpose for HLSL!
#endif
}
#pragma endregion

#pragma region Game Loop

// --------------------------------------------------------
// Update your game here - take input, move objects, etc.
// --------------------------------------------------------
void MyDemoGame::UpdateScene(float deltaTime, float totalTime)
{

		//CubeEntity.setRotationY(totalTime);

		//camera move
		if (GetAsyncKeyState('W') & 0x8000)
		{
			FPScamera.MoveForward(deltaTime * 5);
		}
		if (GetAsyncKeyState('S') & 0x8000)
		{
			FPScamera.MoveBackward(deltaTime * 5);
		}
		if (GetAsyncKeyState('A') & 0x8000)
		{
			FPScamera.MoveLeft(deltaTime * 5);
		}
		if (GetAsyncKeyState('D') & 0x8000)
		{
			FPScamera.MoveRight(deltaTime * 5);
		}
		if (GetAsyncKeyState(VK_SPACE) & 0x8000)
		{
			FPScamera.MoveUp(deltaTime * 5);
		}
		if (GetAsyncKeyState('X') & 0x8000)
		{
			FPScamera.MoveDown(deltaTime * 5);
		}
	// Quit if the escape key is pressed
	if (GetAsyncKeyState(VK_ESCAPE))
		Quit();
}

// --------------------------------------------------------
// Clear the screen, redraw everything, present to the user
// --------------------------------------------------------
void MyDemoGame::DrawScene(float deltaTime, float totalTime)
{
	// Background color (Cornflower Blue in this case) for clearing
	const float color[4] = {0.4f, 0.6f, 0.75f, 0.0f};
	//const float color[4] = { 0.0f, 0.0f, 0.0f, 0.0f };

	// Clear the render target and depth buffer (erases what's on the screen)
	//  - Do this ONCE PER FRAME
	//  - At the beginning of DrawScene (before drawing *anything*)
	deviceContext->ClearRenderTargetView(renderTargetView, color);
	deviceContext->ClearDepthStencilView(
		depthStencilView, 
		D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL,
		1.0f,
		0);


	// Send data to shader variables
	//  - Do this ONCE PER OBJECT you're drawing
	//  - This is actually a complex process of copying data to a local buffer
	//    and then copying that entire buffer to the GPU.  
	//  - The "SimpleShader" class handles all of that for you.
	//vertexShader->SetMatrix4x4("world", worldMatrix);
	//vertexShader->SetMatrix4x4("view", viewMatrix);
	//vertexShader->SetMatrix4x4("projection", projectionMatrix);
	
	// Set the vertex and pixel shaders to use for the next Draw() command
	//  - These don't technically need to be set every frame...YET
	//  - Once you start applying different shaders to different objects,
	//    you'll need to swap the current shaders before each draw
	//vertexShader->SetShader(true);
	//pixelShader->SetShader(true);

	//Camera 
	FPScamera.UpdateVPMatrixes();
	pointlight1.Postion = FPScamera.GetCameraPosition();
	//set light to shader
	pixelShader->SetData("dirlight", &dirlight1, sizeof(DirectionalLight));
	pixelShader->SetData("pointlight", &pointlight1, sizeof(PointLight));
	pixelShader->SetFloat3("cameraPosition", FPScamera.GetCameraPosition());

	pixelShaderST->SetData("dirlight", &dirlight1, sizeof(DirectionalLight));
	pixelShaderST->SetData("pointlight", &pointlight1, sizeof(PointLight));
	pixelShaderST->SetFloat3("cameraPosition", FPScamera.GetCameraPosition());

	pixelShaderReflect->SetData("dirlight", &dirlight1, sizeof(DirectionalLight));
	pixelShaderReflect->SetData("pointlight", &pointlight1, sizeof(PointLight));
	pixelShaderReflect->SetFloat3("cameraPosition", FPScamera.GetCameraPosition());

	//Draw sky box
	SkyBoxEntity.DrawEntity(FPScamera.GetViewMatrix(), FPScamera.GetProjectionMatrix());

	//Entity Draw
	for (int i = 0; i < 1; i++)
	for (int j = 0; j < 1; j++)
	for (int k = 0; k < 3; k++)
	{
		CubeEntity.setPositionX((float)k*4);
		CubeEntity.setPositionY((float)j*4);
		CubeEntity.setPositionZ((float)i*4);
		if (k == 0)
		{
			material1.SetPixelShader(pixelShaderST);
			CubeEntity.DrawEntity(FPScamera.GetViewMatrix(), FPScamera.GetProjectionMatrix());
		}
		else if (k == 2)
		{
			material1.SetPixelShader(pixelShaderReflect);
			CubeEntity.DrawEntity(FPScamera.GetViewMatrix(), FPScamera.GetProjectionMatrix());
		}
	}

	//Draw blending objects last
	CubeEntity.setPositionX((float)1 * 4);
	material1.SetPixelShader(pixelShader);
	CubeEntity.DrawEntity(FPScamera.GetViewMatrix(), FPScamera.GetProjectionMatrix());

	/*********************************************************************
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
		3,     // The number of indices to use (we could draw a subset if we wanted)
		0,     // Offset to the first index we want to use
		0);    // Offset to add to each index when looking up vertices
	********************************************************************/

	// Present the buffer
	//  - Puts the image we're drawing into the window so the user can see it
	//  - Do this exactly ONCE PER FRAME
	//  - Always at the very end of the frame
	HR(swapChain->Present(0, 0));
}

#pragma endregion

#pragma region Mouse Input

// --------------------------------------------------------
// Helper method for mouse clicking.  We get this information
// from the OS-level messages anyway, so these helpers have
// been created to provide basic mouse input if you want it.
//
// Feel free to add code to this method
// --------------------------------------------------------
void MyDemoGame::OnMouseDown(WPARAM btnState, int x, int y)
{
	// Save the previous mouse position, so we have it for the future
	prevMousePos.x = x;
	prevMousePos.y = y;
	// Caputure the mouse so we keep getting mouse move
	// events even if the mouse leaves the window.  we'll be
	// releasing the capture once a mouse button is released
	SetCapture(hMainWnd);
}

// --------------------------------------------------------
// Helper method for mouse release
//
// Feel free to add code to this method
// --------------------------------------------------------
void MyDemoGame::OnMouseUp(WPARAM btnState, int x, int y)
{
	// We don't care about the tracking the cursor outside
	// the window anymore (we're not dragging if the mouse is up)
	ReleaseCapture();
}

// --------------------------------------------------------
// Helper method for mouse movement.  We only get this message
// if the mouse is currently over the window, or if we're 
// currently capturing the mouse.
//
// Feel free to add code to this method
// --------------------------------------------------------
void MyDemoGame::OnMouseMove(WPARAM btnState, int x, int y)
{
	if (btnState & 0x0001)
	{
		//update Cameradirection
		FPScamera.UpdateCameraDir((y - prevMousePos.y)*XM_PIDIV4/500, (x - prevMousePos.x)*XM_PIDIV4/500);
		// Save the previous mouse position, so we have it for the future
		prevMousePos.x = x;
		prevMousePos.y = y;
	}
}

//For other class to get D3DDevice and Context
ID3D11Device* MyDemoGame::GetDevice()
{
	return device;
}

ID3D11DeviceContext* MyDemoGame::GetDevContext()
{
	return deviceContext;
}
//get shaders
SimpleVertexShader* MyDemoGame::GetVertexShader()
{
	return vertexShader;
}

SimplePixelShader* MyDemoGame::GetPixelShader()
{
	return pixelShader;
}
#pragma endregion