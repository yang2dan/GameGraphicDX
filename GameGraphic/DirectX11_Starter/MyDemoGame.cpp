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
	windowCaption = L"My Super Fancy GGP Game";

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
	//Init Material
	//load texture
	CreateWICTextureFromFile(	device, 
								deviceContext, 
								L"orange.jpg", 
								0, 
								&material1.texture);
	//load normalmap
	CreateWICTextureFromFile(device, deviceContext, L"orangenormalmap.jpg", 0, &material1.normalMap);
	//creat sampler state
	D3D11_SAMPLER_DESC samplerDesc = {};
	samplerDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
	samplerDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
	samplerDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
	samplerDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
	samplerDesc.MaxLOD = D3D11_FLOAT32_MAX;

	device->CreateSamplerState(&samplerDesc, &material1.samplerState);

	material1.SetVertexShader(vertexShader);
	material1.SetPixelShader(pixelShader);

	//load mesh
	CreateGeometry();

	//CreateMatrices();

	// Tell the input assembler stage of the pipeline what kind of
	// geometric primitives we'll be using and how to interpret them
	deviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	//Camera Initialize
	FPScamera.SetAspectRatio(aspectRatio);
	
	//Light Initialize
	dirlight1.AmbientColor = XMFLOAT4(0, 0, 0, 1.0);
	dirlight1.DiffuseColor = XMFLOAT4(1, 1, 1, 1.0);
	dirlight1.Direction = XMFLOAT3(1, -1, 0);

	pointlight1.Postion = XMFLOAT3(2, 0, 0);
	pointlight1.Color	 = XMFLOAT4(0.7, 0, 0, 1);

	
	// Successfully initialized
	return true;
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
}


// --------------------------------------------------------
// Creates the geometry we're going to draw - a single triangle for now
// --------------------------------------------------------
void MyDemoGame::CreateGeometry()
{
	/*-------------------------------------------------------------
	// Create some temporary variables to represent colors
	// - Not necessary, just makes things more readable
	XMFLOAT4 red	= XMFLOAT4(1.0f, 0.0f, 0.0f, 1.0f);
	XMFLOAT4 green	= XMFLOAT4(0.0f, 1.0f, 0.0f, 1.0f);
	XMFLOAT4 blue	= XMFLOAT4(0.0f, 0.0f, 1.0f, 1.0f);

	
	XMFLOAT3 normal0 = XMFLOAT3(0, 0, -1);
	XMFLOAT2 uv0	 = XMFLOAT2(0, 0);
	
	//Create a Quadrangle
	Vertex verQuad[] =
	{
		{ XMFLOAT3(-3.5f, +1.0f, +0.0f), normal0, uv0 },
		{ XMFLOAT3(-2.5f, +1.0f, +0.0f), normal0, uv0 },
		{ XMFLOAT3(-3.5f, -1.0f, +0.0f), normal0, uv0 },
		{ XMFLOAT3(-2.5f, -1.0f, +0.0f), normal0, uv0 }
	};

	int indQuad[] = { 0, 3, 2, 0, 1, 3};

	QuadrangleMesh.SetD3DDevice(GetDevice());
	QuadrangleMesh.SetD3DDevContext(GetDevContext());
	QuadrangleMesh.setVerticies(verQuad, 4);
	QuadrangleMesh.setIndices(indQuad, 6);
	QuadrangleMesh.CreateBuffer();

	//Create a Pentagon
	Vertex verPenta[] = 
	{
		{ XMFLOAT3(+0.0f, +1.0f, +0.0f), normal0, uv0 },
		{ XMFLOAT3(+1.0f, +0.5f, +0.0f), normal0, uv0 },
		{ XMFLOAT3(+0.5f, -0.5f, +0.0f), normal0, uv0 },
		{ XMFLOAT3(-0.5f, -0.5f, +0.0f), normal0, uv0 },
		{ XMFLOAT3(-1.0f, +0.5f, +0.0f), normal0, uv0 },
	};

	int indPenta[] = {0, 1, 2, 0, 2, 3, 0, 3, 4};
	PentagonMesh.SetD3DDevice(GetDevice());
	PentagonMesh.SetD3DDevContext(GetDevContext());
	PentagonMesh.setVerticies(verPenta, 5);
	PentagonMesh.setIndices(indPenta, 9);
	PentagonMesh.CreateBuffer();

	//Create a Hexagon
	Vertex verHex[] =
	{
		{ XMFLOAT3(+2.5f, +1.0f, +0.0f), normal0, uv0 },
		{ XMFLOAT3(+3.5f, +0.5f, +0.0f), normal0, uv0 },
		{ XMFLOAT3(+3.5f, -0.5f, +0.0f), normal0, uv0 },
		{ XMFLOAT3(+2.5f, -1.0f, +0.0f), normal0, uv0 },
		{ XMFLOAT3(1.5f, -0.5f, +0.0f),  normal0, uv0 },
		{ XMFLOAT3(1.5f, +0.5f, +0.0f),  normal0, uv0 },
	};

	int indHex[] = { 0, 1, 5, 5, 1, 3, 1, 2, 3, 5, 3, 4 };
	HexagonMesh.SetD3DDevice(GetDevice());
	HexagonMesh.SetD3DDevContext(GetDevContext());
	HexagonMesh.setVerticies(verHex, 6);
	HexagonMesh.setIndices(indHex, 12);
	HexagonMesh.CreateBuffer();

	//Set Entity
	PentagonEntity.setMesh(&PentagonMesh);
	PentagonEntity.setMaterial(&material1);
	----------------------------------------------------------------*/
	//Load obj file
	CubeMesh.SetD3DDevice(GetDevice());
	CubeMesh.SetD3DDevContext(GetDevContext());
	CubeMesh.LoadObjFile("sphere.obj");


	CubeEntity.setMesh(&CubeMesh);
	CubeEntity.setMaterial(&material1);
}


// --------------------------------------------------------
// Initializes the matrices necessary to represent our geometry's 
// transformations and our 3D camera
// --------------------------------------------------------
void MyDemoGame::CreateMatrices()
{
#if 0
	// Set up world matrix
	// - In an actual game, each object will need one of these and they should
	//   update when/if the object moves (every frame)
	
	XMMATRIX W = XMMatrixIdentity();
	XMStoreFloat4x4(&worldMatrix, XMMatrixTranspose(W)); // Transpose for HLSL!
    
	// Create the View matrix
	// - In an actual game, recreate this matrix when the camera 
	//    moves (potentially every frame)
	// - We're using the LOOK TO function, which takes the position of the
	//    camera and the direction you want it to look (as well as "up")
	// - Another option is the LOOK AT function, to look towards a specific
	//    point in 3D space
	XMVECTOR pos = XMVectorSet(0, 0, -10, 0);
	XMVECTOR dir = XMVectorSet(0, 0, 1, 0);
	XMVECTOR up  = XMVectorSet(0, 1, 0, 0);
	XMMATRIX V   = XMMatrixLookToLH(
		pos,     // The position of the "camera"
		dir,     // Direction the camera is looking
		up);     // "Up" direction in 3D space (prevents roll)
	XMStoreFloat4x4(&viewMatrix, XMMatrixTranspose(V)); // Transpose for HLSL!

	// Create the Projection matrix
	// - This should match the window's aspect ratio, and also update anytime
	//   the window resizes (which is already happening in OnResize() below)
	XMMATRIX P = XMMatrixPerspectiveFovLH(
		0.25f * 3.1415926535f,		// Field of View Angle
		aspectRatio,				// Aspect ratio
		0.1f,						// Near clip plane distance
		100.0f);					// Far clip plane distance
	XMStoreFloat4x4(&projectionMatrix, XMMatrixTranspose(P)); // Transpose for HLSL!
#endif
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
float x = 0;
int test1 = 0;
float timeclock = 0;
void MyDemoGame::UpdateScene(float deltaTime, float totalTime)
{

	timeclock += deltaTime;
	if (timeclock > (1.0/60))
	{

		test1 = test1++ % 628;
//		CubeEntity.setPositionX(3*sin((float)test1 / 100));
//		CubeEntity.setPositionY(cos((float)test1 / 50));
		//CubeEntity.setRotationX((float)test1 / 50);
		CubeEntity.setRotationY((float)test1 / 50);

//		PentagonEntity.setRotationZ((float)test1 / 10);
//		PentagonEntity.setScaleX(sin((float)test1 / 100));
//		PentagonEntity.setScaleY(sin((float)test1 / 100));

		//camera move
		if (GetAsyncKeyState('W') & 0x8000)
		{
			FPScamera.MoveForward();
		}
		if (GetAsyncKeyState('S') & 0x8000)
		{
			FPScamera.MoveBackward();
		}
		if (GetAsyncKeyState('A') & 0x8000)
		{
			FPScamera.MoveLeft();
		}
		if (GetAsyncKeyState('D') & 0x8000)
		{
			FPScamera.MoveRight();
		}
		if (GetAsyncKeyState(VK_SPACE) & 0x8000)
		{
			FPScamera.MoveUp();
		}
		if (GetAsyncKeyState('X') & 0x8000)
		{
			FPScamera.MoveDown();
		}
		timeclock = 0;
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
	//const float color[4] = {0.4f, 0.6f, 0.75f, 0.0f};
	const float color[4] = { 0.0f, 0.0f, 0.0f, 0.0f };

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

	//set light to shader
	pixelShader->SetData("dirlight", &dirlight1, sizeof(DirectionalLight));
	pixelShader->SetData("pointlight", &pointlight1, sizeof(PointLight));
	pixelShader->SetFloat3("cameraPosition", FPScamera.GetCameraPosition());
	//Entity Draw	
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
		FPScamera.UpdateCameraDir((y - prevMousePos.y)*XM_PIDIV4/1000, (x - prevMousePos.x)*XM_PIDIV4/1000);
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