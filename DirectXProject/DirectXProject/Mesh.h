#pragma once

#include "Component.h"
#include <D3D11.h>
#include <string>

struct MeshData {
	ID3D11Buffer *m_vertexBuffer, *m_indexBuffer;
	int m_vertexCount, m_indexCount;
};


class Mesh : public Component<Mesh>
{
public:
	Mesh();
	~Mesh();

	MeshData GetGeometry();
private:
	MeshData geometry;
	std::string filepath;
};