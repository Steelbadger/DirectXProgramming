#include "ObjectFactory.h"
#include "World.h"
#include "RenderData.h"
#include "D3DRenderer.h"
#include <D3DX11.h>
#include <iostream>

#include "UtilityFunctions.h"



ObjectFactory::ObjectFactory()
{

}

ObjectFactory::~ObjectFactory()
{

}

void ObjectFactory::SetGraphicsContext(DirectXContext& context)
{
	graphicsContext = &context;
}

void ObjectFactory::SetWorld(World& world)
{
	targetWorld = &world;
}

void ObjectFactory::SetRenderer(D3DRenderer& renderer)
{
	targetRenderer = &renderer;
}
	

ObjectID ObjectFactory::CreateNewObject(World& world, std::string modelFileString, std::string shaderFile, std::string ambientTexture,
											std::string normalMap, std::string specularMap)
{
	GameObject result;
	RenderData objectData;
	HRESULT check;
	ID3D10Blob* VS_Buffer;
	ID3D10Blob* PS_Buffer;

	//Compile Shaders from shader file
	check = D3DX11CompileFromFile("Effects.fx", 0, 0, "VertexShader", "vs_5_0", 0, 0, 0, &VS_Buffer, 0, 0);
	if(FAILED(check)) {
		Error("Failed to Compile Vertex Shader From File");
	}
	check = D3DX11CompileFromFile("Effects.fx", 0, 0, "PixelShader", "ps_5_0", 0, 0, 0, &PS_Buffer, 0, 0);
	if(FAILED(check)) {
		Error("Failed to Compile Pixel Shader From File");
	}

	//Create the Shader Objects
	check = graphicsContext->GetDevice()->CreateVertexShader(VS_Buffer->GetBufferPointer(), VS_Buffer->GetBufferSize(), NULL, &objectData.vertexShader);
	if(FAILED(check)) {
		Error("Failed to Create Vertex Shader Object");
	}
	check = graphicsContext->GetDevice()->CreatePixelShader(PS_Buffer->GetBufferPointer(), PS_Buffer->GetBufferSize(), NULL, &objectData.pixelShader);
	if(FAILED(check)) {
		Error("Failed to Create Pixel Shader Object");
	}


	objectData.numTextures = 1;


	check = D3DX11CreateShaderResourceViewFromFile(graphicsContext->GetDevice(), ambientTexture.c_str(), NULL, NULL, &objectData.textures[0], NULL);
	if(FAILED(check)) {
		Error("Failed to Create Shader Resource for Ambient Texture");
	}

	if (!normalMap.empty()) {
		check = D3DX11CreateShaderResourceViewFromFile(graphicsContext->GetDevice(), normalMap.c_str(), NULL, NULL, &objectData.textures[1], NULL);
		if(FAILED(check)) {
			Error("Failed to Create Shader Resource for Normal Map");
		}
		objectData.numTextures++;
	}

	if (!specularMap.empty()) {
		check = D3DX11CreateShaderResourceViewFromFile(graphicsContext->GetDevice(), specularMap.c_str(), NULL, NULL, &objectData.textures[2], NULL);
		if(FAILED(check)) {
			Error("Failed to Create Shader Resource for Specular Map");
		}
		objectData.numTextures++;
	}

	LoadModel(objectData, modelFileString);
	
	
	//Create the Input Layout
	graphicsContext->GetDevice()->CreateInputLayout( layout, LAYOUTELEMENTS, VS_Buffer->GetBufferPointer(), 
														VS_Buffer->GetBufferSize(), &objectData.vertLayout );

	result.orientationID = targetWorld->orientations.Add(D3DXQUATERNION(0, 1, 0, 0));

	D3DVECTOR pos;
	pos.x = 0;
	pos.y = 0;
	pos.z = 0;
	result.positionID = targetWorld->positions.Add(pos);


	ObjectID output = targetWorld->objects.Add(result);
	objectData.objectID = output;

	targetRenderer->data.Add(objectData);

	return output;
}

void ObjectFactory::LoadModel(RenderData& output, std::string filename)
{
	std::vector<unsigned int> vertIndices, uvIndices, normalIndices;
	std::vector<Vector3> tempVerts;
	std::vector<Vector3> tempNormals;
	std::vector<Vector2> tempUVs;

	std::vector<Vector3> verts;
	std::vector<Vector3> normals;
	std::vector<Vector2> uvs;
	std::vector<unsigned int> index;

	const char* path = filename.c_str();

	FILE * file = fopen(path, "r");
	if( file == NULL ){
		Error("Cannot Open File: " + filename);
		return;
	}

	while(true){

		char lineHeader[128];
		// read the first word of the line
		int res = fscanf(file, "%s", lineHeader);
		if (res == EOF)
			break; // EOF = End Of File. Quit the loop.

		// else : parse lineHeader
		
		if ( strcmp( lineHeader, "v" ) == 0 ){
			Vector3 vertex;
			fscanf(file, "%f %f %f\n", &vertex.x, &vertex.y, &vertex.z );
			tempVerts.push_back(vertex);
		} else if ( strcmp( lineHeader, "vt" ) == 0 ){
			Vector2 uv;
			fscanf(file, "%f %f\n", &uv.u, &uv.v );
			tempUVs.push_back(uv);
		} else if ( strcmp( lineHeader, "vn" ) == 0 ){
			Vector3 normal;
			fscanf(file, "%f %f %f\n", &normal.x, &normal.y, &normal.z );
			tempNormals.push_back(normal);
		} else if ( strcmp( lineHeader, "f" ) == 0 ){
			std::string vertex1, vertex2, vertex3;
			unsigned int vertexIndex[3], uvIndex[3], normalIndex[3];
			int matches = fscanf(file, "%d/%d/%d %d/%d/%d %d/%d/%d\n", &vertexIndex[0], &uvIndex[0], &normalIndex[0], &vertexIndex[1], &uvIndex[1], &normalIndex[1], &vertexIndex[2], &uvIndex[2], &normalIndex[2] );
			if (matches != 9){
				Error("Cannot Read File: " + filename);
				fclose(file);
				return;
			}
			vertIndices.push_back(vertexIndex[0]);
			vertIndices.push_back(vertexIndex[1]);
			vertIndices.push_back(vertexIndex[2]);
			uvIndices.push_back(uvIndex[0]);
			uvIndices.push_back(uvIndex[1]);
			uvIndices.push_back(uvIndex[2]);
			normalIndices.push_back(normalIndex[0]);
			normalIndices.push_back(normalIndex[1]);
			normalIndices.push_back(normalIndex[2]);
		} else {
			// Probably a comment, eat up the rest of the line
			char stuff[1000];
			fgets(stuff, 1000, file);
		}

	}

	// For each vertex of each triangle
	for( unsigned int i=0; i<vertIndices.size(); i++ ){

		// Get the indices of its attributes
		unsigned int vertexIndex = vertIndices[i];
		unsigned int uvIndex = uvIndices[i];
		unsigned int normalIndex = normalIndices[i];
		
		// Get the attributes thanks to the index
		Vector3 vertex = tempVerts[vertexIndex-1];
		Vector2 uv = tempUVs[uvIndex-1];
		Vector3 normal = tempNormals[normalIndex-1];
		
		// Put the attributes in buffers
		verts.push_back(vertex);
		uvs.push_back(uv);
		normals.push_back(normal);
		index.push_back(i);
	}
	fclose(file);

	if (false) {
		Indexify(verts, uvs, normals, index);
	}

	ConstructBuffers(output, verts, uvs, normals, index);

	return;
}

void ObjectFactory::Indexify(std::vector<Vector3>& verts, std::vector<Vector2>& uvs, std::vector<Vector3>& normals, std::vector<unsigned int>& index)
{
	bool duplicateFound = false;
	std::vector<Vector3> tempVerts;
	std::vector<Vector2> tempUVs;
	std::vector<Vector3> tempNormals;
	
	for (int i = 0; i < index.size(); i++) {
		duplicateFound = false;
		for (int j = 0; j < i; j++) {
			if (verts[index[i]] == verts[index[j]] && uvs[index[i]] == uvs[index[j]] && normals[index[i]] == normals[index[j]]) {
				index[i] = index[j];
				duplicateFound = true;
			}
		}
		if (duplicateFound == false) {
			tempVerts.push_back(verts[index[i]]);
			tempUVs.push_back(uvs[index[i]]);
			tempNormals.push_back(normals[index[i]]);
		}
	}

	verts = tempVerts;
	normals = tempNormals;
	uvs = tempUVs;

	return;
}

void ObjectFactory::ConstructBuffers(RenderData& output, std::vector<Vector3>& verts, std::vector<Vector2>& uvs,
										std::vector<Vector3>& normals, std::vector<unsigned int>& index)
{
	D3D11_BUFFER_DESC indexBufferDesc;
	ZeroMemory( &indexBufferDesc, sizeof(indexBufferDesc) );

	indexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	indexBufferDesc.ByteWidth = sizeof(unsigned int) * index.size();
	indexBufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
	indexBufferDesc.CPUAccessFlags = 0;
	indexBufferDesc.MiscFlags = 0;

	D3D11_SUBRESOURCE_DATA initData;

	initData.pSysMem = (void*)&index[0];
	graphicsContext->GetDevice()->CreateBuffer(&indexBufferDesc, &initData, &output.indexBuffer);



	struct Vertex {
		Vector3 pos;
		Vector2 uv;
		Vector3 normal;
	};

	output.stride = sizeof(Vertex);

	std::vector<Vertex> vertices;
	vertices.resize(verts.size());

	for (int i = 0; i < vertices.size(); i++) {
		vertices[i].pos = verts[i];
		vertices[i].uv = uvs[i];
		vertices[i].normal = normals[i];
	}

	D3D11_BUFFER_DESC vertexBufferDesc;
	ZeroMemory( &vertexBufferDesc, sizeof(vertexBufferDesc) );

	vertexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	vertexBufferDesc.ByteWidth = sizeof( Vertex ) * vertices.size();
	vertexBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	vertexBufferDesc.CPUAccessFlags = 0;
	vertexBufferDesc.MiscFlags = 0;

	D3D11_SUBRESOURCE_DATA vertexBufferData; 

	ZeroMemory( &vertexBufferData, sizeof(vertexBufferData) );
	vertexBufferData.pSysMem = (void*)&vertices[0];
	graphicsContext->GetDevice()->CreateBuffer( &vertexBufferDesc, &vertexBufferData, &output.vertBuffer);
}