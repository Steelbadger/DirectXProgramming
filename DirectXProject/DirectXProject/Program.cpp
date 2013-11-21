#include "program.h"
#include "Hardware.h"
#include "DirectXContext.h"
#include <iostream>

Program::Program(void)
{
	hardware = &HardwareState::GetInstance();
}


Program::~Program(void)
{
}

void Program::Initialize(HINSTANCE hInstance)
/*-------------------------------------------------------------------------*\
|	Purpose:	Create a window, initialise OpenGL context and retrieve		|
|				machine and driver specs									|
|																			|
|	Parameters:	hInstance of the process									|
|																			|
|	Returns:	None.														|
|																			|
\*-------------------------------------------------------------------------*/
{

	//  Create a new window, 640 pix long and 512 pix tall
	window.Create("Basic Program", 640, 480, (WS_OVERLAPPEDWINDOW | WS_CLIPSIBLINGS | WS_CLIPCHILDREN), false, hInstance);
	//window.CreateFullscreen("Basic Program", hInstance);
	context = new DirectXContext();

	//  Move the cursor to the middle of the window
	window.SetCursorToCentre();
	context->Initialize(window);
	renderer.Initialize(window);
	world.Initialise();

	factory.SetGraphicsContext(*context);
	factory.SetWorld(world);
	factory.SetRenderer(renderer);


	//  Now try adding assets!

	factory.CreateNewObject(world, "outwardCube.obj", "Effects.fx", "seafloor.dds");
	renderer.Load(world);
}

void Program::MainLoop()
/*-------------------------------------------------------------------------*\
|	Purpose:	The primary application loop, handles calling of all		|
|				update functions											|
|																			|
|	Parameters:	None.														|
|																			|
|	Returns:	None.														|
|																			|
\*-------------------------------------------------------------------------*/
{
	//  Slow down incase of really fast machine
	if (hardware->GetTimeSinceLastFrame() >= 10) {
		//  Update the input state
		hardware->Update();
		context->BeginScene();

		renderer.Render();
		//  UI.Draw();

		context->EndScene();
	}

	//  Exit the program
	if (hardware->Pressed(VK_ESCAPE))
		exit(0);
}
