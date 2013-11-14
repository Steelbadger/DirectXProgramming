#pragma once
#include <Windows.h>

class Window;


class GraphicsContext
{
public:
	GraphicsContext(void){}
	virtual ~GraphicsContext(void){}
	virtual void Initialize(Window& window) = 0;
	virtual void FlipBuffers() = 0;


private:
	static const int COLOUR_DEPTH = 16;
};

