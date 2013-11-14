#pragma once
//  Author - Ross Davies
//  This class turns the GameObject derived class to which
//  it is attached into an OpenGL camera.  There are no functions
//  for moving the camera, any movement on the camera is handled 
//  by the parent GameObject.

#include "Vector4.h"
#include "GraphicsWindow.h"
#include "Matrix4x4.h"

#include <windows.h>

class GameObject;

class Camera
{
public:
	Camera(GameObject* parent);
	~Camera(void);

	void SetTargetWindow(Window* window);

	void SetWindowSize(int width, int height);
	void SetWindowSize();

	void SetClipPlanes(float near, float far);

	void SetFieldOfView(float fov);

	float GetFieldOfView(){return fieldOfView;}

	float GetAspectRatio(){return float(windowWidth/windowHeight);}

	void LockCursorToCentre();

	int GetWindowWidth(){return windowWidth;}
	int GetWindowHeight(){return windowHeight;}

	float GetNearClipPlane(){return nearClipPlane;}
	float GetFarClipPlane(){return farClipPlane;}

	Vector4 GetForward();
	Vector4 GetUp();
	Vector4 GetPosition();

	GameObject* GetParent(){return parent;}

	void Update();

	void Zoom(int zoom);

private:
	GameObject* parent;
	Window* targetWindow;

	int windowWidth;
	int windowHeight;
	float fieldOfView;
	float nearClipPlane;
	float farClipPlane;

	Matrix4x4 model;
	Matrix4x4 projection;

	Vector4 LookAt;
};

