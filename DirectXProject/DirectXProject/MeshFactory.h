#pragma once

#include "Mesh.h"
#include <D3D11.h>
#include <D3DX10math.h>
#include <vector>
#include <map>


class MeshFactory
{
private:
	struct BasicVertexType
	{
		D3DXVECTOR3 position;
		D3DXVECTOR2 texture;
	};

	struct LitVertexType
	{
		D3DXVECTOR3 position;
		D3DXVECTOR2 texture;
		D3DXVECTOR3 normal;
	};

	struct MappedVertexType
	{
		D3DXVECTOR3 position;
		D3DXVECTOR2 texture;
		D3DXVECTOR3 normal;
		D3DXVECTOR3 tangent;
		D3DXVECTOR3 bitangent;
	};

public:
	MeshFactory();
	~MeshFactory();

	MeshData CreateMeshBuffersFromFile(std::string filename, Mesh::FeatureLevel featurelevel);
	void SetDevice(ID3D11Device* device);
private:

	MeshData CreateBasicMeshBuffersFromFile(std::string filename);
	MeshData CreateLitMeshBuffersFromFile(std::string filename);
	MeshData CreateMappedMeshBuffersFromFile(std::string filename);

	void LoadBasicVerts(std::string filename, std::vector<BasicVertexType>& outverts, std::vector<unsigned int>& outindex);
	void LoadLitVerts(std::string filename, std::vector<LitVertexType>& outverts, std::vector<unsigned int>& outindex);
	std::vector<MappedVertexType> ComputeTangentSpace(const std::vector<LitVertexType>& data, const std::vector<unsigned int>& index);

	void LoadObj(std::string filename, std::vector<LitVertexType>& outverts, std::vector<unsigned int>& outindex);

	ID3D11Device* device;

	std::map<std::string, MeshData> loadedMeshMap;
};