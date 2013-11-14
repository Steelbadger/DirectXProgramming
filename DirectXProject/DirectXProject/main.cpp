#include "Program.h"

#include <Windows.h>

int main (HINSTANCE hInstance, HINSTANCE hPrevInstance,
                    PSTR szCmdLine, int nCmdShow)			
{	
    MSG msg;
	Program application;

	application.Initialize(hInstance);

	while (true) {							
		if (PeekMessage(&msg,NULL,0,0,PM_REMOVE)) {
		    if (msg.message==WM_QUIT)
				break;
			TranslateMessage (&msg);		
			DispatchMessage (&msg);
		} else {	
			application.MainLoop();
		}
    }
	return msg.wParam ;										
}