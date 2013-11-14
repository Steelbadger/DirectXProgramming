#pragma once

#include "Mesh.h"
#include "Camera.h"
#include "Controller.h"


#define TOTALCOMPONENTS	5

#define	POSITION		0x01
#define ORIENTATION		0x02
#define SCALE			0x04
#define MESH			0x08
#define CAMERA			0x10
#define CONTROLLER		0x20

#define MESHOBJECT		(POSITION | ORIENTATION | SCALE | MESH)
#define CAMERAOBJECT	(POSITION | ORIENTATION | CAMERA | CONTROLLER)
#define ABSTRACTOBJECT	0x00

