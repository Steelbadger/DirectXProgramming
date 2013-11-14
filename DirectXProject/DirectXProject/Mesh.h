#pragma once

#include "material.h"
#include "Vector3.h"
#include "Vector2.h"

#include <map>
#include <vector>
#include <windows.h>
#include <stdio.h>
#include <string>

class GameObject;


class Mesh
{
public:
	Mesh(const char* meshPath);
	Mesh(std::vector<Vector3> verts, std::vector<Vector3> normals, std::vector<Vector2> uvs);
	Mesh(std::vector<Vector3> verts, std::vector<Vector3> normals, std::vector<Vector2> uvs, std::vector<unsigned int> index);
	Mesh();
	~Mesh(void);

	void AttachMaterial(Material m){material = m;}
	Material GetMaterial(){return material;}

	void GetNewUniqueID();
	Vector3* GetVertexArrayBase(){return &verts[0];}
	Vector3* GetNormalArrayBase(){return &normals[0];}
	Vector2* GetUVArrayBase(){return &uvs[0];}
	unsigned int* GetIndexArrayBase(){return &index[0];}
	std::string GetMeshSourceFilePath(){return meshPath;}
	int GetNumberOfVerts(){return numVerts;}
	int GetSizeOfVerts() {return verts.size()*sizeof(float)*3;}
	int GetSizeOfNormals() {return normals.size()*sizeof(float)*3;}
	int GetSizeOfUVs() {return uvs.size()*sizeof(float)*2;}
	int GetIndexLength(){return index.size();}
	void ReverseNormals();

	std::vector<Vector3> GetVerts(){return verts;}
	std::vector<Vector3> GetNormals(){return normals;}
	std::vector<Vector2> GetUVs(){return uvs;}
	std::vector<unsigned int> GetIndex(){return index;}

	void DeleteVertexData();

	Mesh& operator=(const Mesh& m);

protected:
	std::vector<Vector3> verts;
	std::vector<Vector3> normals;
	std::vector<Vector2> uvs;
	std::vector<unsigned int> index;

private:
	bool LoadMesh(const char* path);
	bool LoadObj(const char* path);

	int numVerts;

	std::string meshPath;

	Material material;
};