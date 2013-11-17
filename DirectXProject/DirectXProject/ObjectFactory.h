#pragma once
#include "LookupTable.h"
#include <map>
#include "MathsClasses.h"
#include <vector>

class World;
class GameObject;
class D3DRenderer;
class DirectXContext;
class RenderData;

class ObjectFactory
{
public:
	ObjectFactory();
	~ObjectFactory();

	ObjectID CreateNewObject(World& world, std::string modelFileString, std::string shaderFile, std::string ambientTexture,
								std::string normalMap = "", std::string specularMap = "");

	void SetGraphicsContext(DirectXContext& context);
	void SetWorld(World& world);
	void SetRenderer(D3DRenderer& renderer);
	
private:
	//  Maps to reduce loading times
	std::map<std::string, ObjectID> ModelFileToVertexBuffer;
	std::map<std::string, ObjectID> ShaderFileToShaderBuffer;
	std::map<std::string, ObjectID> AmbientFileToTextureResource;
	std::map<std::string, ObjectID> NormalFileToTextureResource;
	std::map<std::string, ObjectID> SpecularFileToTextureResource;

	DirectXContext* graphicsContext;
	World* targetWorld;
	D3DRenderer* targetRenderer;


private:
	//  Helper Functions
	void LoadModel(RenderData& output, std::string filename);
	void Indexify(std::vector<Vector3>& verts, std::vector<Vector2>& uvs, std::vector<Vector3>& normals, std::vector<unsigned int>& index);
	void ConstructBuffers(RenderData& output, std::vector<Vector3>& verts, std::vector<Vector2>& uvs,
							std::vector<Vector3>& normals, std::vector<unsigned int>& index);
};
