#include "Application.h"


int main(HINSTANCE hInstance, HINSTANCE hPrevInstance, PSTR pScmdline, int iCmdshow)
{
//	SystemClass* System;
	Application* program;
	bool result;
	
	
	// Create the system object.
	//System = new SystemClass;
	//if(!System)
	//{
	//	return 0;
	//}

	program = new Application;
	if(!program)
	{
		return 0;
	}

	// Initialize and run the system object.
	result = program->Initialize();
	if(result)
	{
		program->Run();
	}

	// Shutdown and release the system object.
	program->Shutdown();
	delete program;
	program = 0;

	return 0;
}
