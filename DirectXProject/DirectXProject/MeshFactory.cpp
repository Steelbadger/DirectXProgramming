#include "MeshFactory.h"
#include "UtilityFunctions.h"


MeshFactory::MeshFactory()
{
}


MeshFactory::~MeshFactory()
{
}

void MeshFactory::SetDevice(ID3D11Device* dev)
{
	device = dev;
}

MeshData MeshFactory::CreateMeshBuffersFromFile(std::string filename, Mesh::FeatureLevel features)
{
	MeshData output;
	if (loadedMeshMap.count(filename) != 0) {
		output = loadedMeshMap[filename];
	} else {

		switch (features) {
		case Mesh::FeatureLevel::TEXTURED:
			output = CreateBasicMeshBuffersFromFile(filename);
			break;
		case Mesh::FeatureLevel::LIT:
			output = CreateLitMeshBuffersFromFile(filename);
			break;
		case Mesh::FeatureLevel::NORMALMAPPED:
			output = CreateMappedMeshBuffersFromFile(filename);
			break;
		}
		loadedMeshMap[filename] = output;
	}
	return output;
}

MeshData MeshFactory::CreateBasicMeshBuffersFromFile(std::string filename)
{
	std::vector<BasicVertexType> verts;
	std::vector<unsigned int> index;
	LoadBasicVerts(filename, verts, index);

	MeshData output;

	output.stride = sizeof(BasicVertexType);

	D3D11_BUFFER_DESC vertexBufferDesc, indexBufferDesc;
	D3D11_SUBRESOURCE_DATA vertexData, indexData;
	HRESULT result;

	output.m_indexCount = index.size();
	output.m_vertexCount = verts.size();

	// Set up the description of the static vertex buffer.
	vertexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	vertexBufferDesc.ByteWidth = sizeof(BasicVertexType) * output.m_vertexCount;
	vertexBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	vertexBufferDesc.CPUAccessFlags = 0;
	vertexBufferDesc.MiscFlags = 0;
	vertexBufferDesc.StructureByteStride = 0;

	// Give the subresource structure a pointer to the vertex data.
	vertexData.pSysMem = &verts[0];
	vertexData.SysMemPitch = 0;
	vertexData.SysMemSlicePitch = 0;

	// Now create the vertex buffer.
	result = device->CreateBuffer(&vertexBufferDesc, &vertexData, &output.m_vertexBuffer);
	if(FAILED(result))
	{
		Error("Failed to Create Vertex Buffer for file: " + filename);
	}

	// Set up the description of the static index buffer.
	indexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	indexBufferDesc.ByteWidth = sizeof(unsigned int) * output.m_indexCount;
	indexBufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
	indexBufferDesc.CPUAccessFlags = 0;
	indexBufferDesc.MiscFlags = 0;
	indexBufferDesc.StructureByteStride = 0;

	// Give the subresource structure a pointer to the index data.
	indexData.pSysMem = &index[0];
	indexData.SysMemPitch = 0;
	indexData.SysMemSlicePitch = 0;

	// Create the index buffer.
	result = device->CreateBuffer(&indexBufferDesc, &indexData, &output.m_indexBuffer);
	if(FAILED(result))
	{
		Error("Failed to Create Index Buffer for file: " + filename);
	}

	return output;
}

MeshData MeshFactory::CreateLitMeshBuffersFromFile(std::string filename)
{
	std::vector<LitVertexType> verts;
	std::vector<unsigned int> index;
	LoadLitVerts(filename, verts, index);

	MeshData output;

	output.m_vertexCount = verts.size();
	output.m_indexCount = index.size();
	output.stride = sizeof(LitVertexType);
	output.topology = D3D_PRIMITIVE_TOPOLOGY::D3D10_PRIMITIVE_TOPOLOGY_TRIANGLELIST;

	D3D11_BUFFER_DESC vertexBufferDesc, indexBufferDesc;
	D3D11_SUBRESOURCE_DATA vertexData, indexData;
	HRESULT result;

	//////////////////////////////////////////////////////////////////////////////////////
	//////////////////////////////////////TEST STUFF//////////////////////////////////////

	//// Set the number of vertices in the vertex array.
	//output.m_vertexCount = 4;

	//// Set the number of indices in the index array.
	//output.m_indexCount = 4;

	//verts.resize(4);
	//output.topology = D3D_PRIMITIVE_TOPOLOGY::D3D10_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP;

	//// Load the vertex array with data.
	//verts[0].position = D3DXVECTOR3(-1.0f, -1.0f, 0.0f);  // Bottom Left.
	//verts[0].texture = D3DXVECTOR2(0.0f, 0.0f);
	//verts[0].normal = D3DXVECTOR3(0.0f, 0.0f, -1.0f);

	//verts[1].position = D3DXVECTOR3(-1.0f, 1.0f, 0.0f);  // Top Left.
	//verts[1].texture = D3DXVECTOR2(0.0f, 1.0f);
	//verts[1].normal = D3DXVECTOR3(0.0f, 0.0f, -1.0f);
	//
	//verts[2].position = D3DXVECTOR3(1.0f, -1.0f, 0.0f);  // Bottom Right.
	//verts[2].texture = D3DXVECTOR2(1.0f, 0.0f);
	//verts[2].normal = D3DXVECTOR3(0.0f, 0.0f, -1.0f);

	//verts[3].position = D3DXVECTOR3(1.0f, 1.0f, 0.0f);  // Top Right.
	//verts[3].texture = D3DXVECTOR2(1.0f, 1.0f);
	//verts[3].normal = D3DXVECTOR3(0.0f, 0.0f, -1.0f);

	//index.resize(4);

	//// Load the index array with data.
	//index[0] = 0;  // Bottom left.
	//index[1] = 1;  // Top left.
	//index[2] = 2;  // Top right.
	//index[3] = 3;  // Top right.

	//////////////////////////////////END OF TEST STUFF///////////////////////////////////
	//////////////////////////////////////////////////////////////////////////////////////


	// Set up the description of the static vertex buffer.
	vertexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	vertexBufferDesc.ByteWidth = sizeof(LitVertexType) * output.m_vertexCount;
	vertexBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	vertexBufferDesc.CPUAccessFlags = 0;
	vertexBufferDesc.MiscFlags = 0;
	vertexBufferDesc.StructureByteStride = 0;

	// Give the subresource structure a pointer to the vertex data.
	vertexData.pSysMem = &verts[0];
	vertexData.SysMemPitch = 0;
	vertexData.SysMemSlicePitch = 0;

	// Now create the vertex buffer.
	result = device->CreateBuffer(&vertexBufferDesc, &vertexData, &output.m_vertexBuffer);
	if(FAILED(result))
	{
		Error("Failed to Create Vertex Buffer for file: " + filename);
	}

	// Set up the description of the static index buffer.
	indexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	indexBufferDesc.ByteWidth = sizeof(unsigned int) * output.m_indexCount;
	indexBufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
	indexBufferDesc.CPUAccessFlags = 0;
	indexBufferDesc.MiscFlags = 0;
	indexBufferDesc.StructureByteStride = 0;

	// Give the subresource structure a pointer to the index data.
	indexData.pSysMem = &index[0];
	indexData.SysMemPitch = 0;
	indexData.SysMemSlicePitch = 0;

	// Create the index buffer.
	result = device->CreateBuffer(&indexBufferDesc, &indexData, &output.m_indexBuffer);
	if(FAILED(result))
	{
		Error("Failed to Create Index Buffer for file: " + filename);
	}

	return output;
}

MeshData MeshFactory::CreateMappedMeshBuffersFromFile(std::string filename)
{
	MeshData output;
	return output;
}

void MeshFactory::LoadBasicVerts(std::string filename, std::vector<BasicVertexType>& outverts, std::vector<unsigned int>& outindex)
{
	std::vector<BasicVertexType> result;
	std::vector<LitVertexType> loaded;
	std::vector<unsigned int> index;

	LoadObj(filename, loaded, index);

	for (int i = 0; i < loaded.size(); i++) {
		BasicVertexType vert;
		vert.position = loaded[i].position;
		vert.texture = loaded[i].texture;
		result.push_back(vert);
	}


}

void MeshFactory::LoadLitVerts(std::string filename, std::vector<LitVertexType>& outverts, std::vector<unsigned int>& outindex)
{
	LoadObj(filename, outverts, outindex);
}

std::vector<MeshFactory::MappedVertexType> MeshFactory::ComputeTangentSpace(const std::vector<LitVertexType>& data, const std::vector<unsigned int>& index)
{
	std::vector<MappedVertexType> thingy;
	return thingy;
}

void MeshFactory::LoadObj(std::string filename, std::vector<LitVertexType>& outverts, std::vector<unsigned int>& outindex)
{
	std::vector<unsigned int> vertIndices, uvIndices, normalIndices;
	std::vector<D3DXVECTOR3> tempVerts;
	std::vector<D3DXVECTOR3> tempNormals;
	std::vector<D3DXVECTOR2> tempUVs;

	std::vector<unsigned int> index;
	std::vector<LitVertexType> vertexs;

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
			D3DXVECTOR3 vertex;
			fscanf(file, "%f %f %f\n", &vertex.x, &vertex.y, &vertex.z );
			tempVerts.push_back(vertex);
		} else if ( strcmp( lineHeader, "vt" ) == 0 ){
			D3DXVECTOR2 uv;
			fscanf(file, "%f %f\n", &uv.x, &uv.y );
			tempUVs.push_back(uv);
		} else if ( strcmp( lineHeader, "vn" ) == 0 ){
			D3DXVECTOR3 normal;
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
	fclose(file);

	// For each vertex of each triangle
	for( unsigned int i=0; i<vertIndices.size(); i++ ){

		// Get the indices of its attributes
		unsigned int vertexIndex = vertIndices[i];
		unsigned int uvIndex = uvIndices[i];
		unsigned int normalIndex = normalIndices[i];
		
		// Get the attributes thanks to the index
		LitVertexType v;
		v.position = tempVerts[vertexIndex-1];;
		v.texture = tempUVs[uvIndex-1];
		v.normal = tempNormals[normalIndex-1];

		// Put the attributes in buffers
		vertexs.push_back(v);
		index.push_back(i);
	}

	outverts = vertexs;
	outindex = index;
}