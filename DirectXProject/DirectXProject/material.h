#pragma once
///////////////////////// By Ross Davies /////////////////////////
//	The material class which contains all material properties	//
//	of the mesh with which it is associated; the textures		//
//	and shader programs.										//
//////////////////////////////////////////////////////////////////

#include <vector>
#include <map>

#include "texture.h"
class Shader;

class Material
{
public:
	Material(void);
	Material(std::string name);
	~Material(void);

	//  Attach a texture object to the material
	void AddTexture(Texture t);
	//  Create and add a shader (from file) to the material
	void AddShader(std::string shader);
	//  Remove oldTex and replace it with newTex
	void ReplaceTexture(Texture oldTex, Texture newTex);
	//  Replace the first texture of type t with the new texture
	void ReplaceTexture(Texture::Type t, Texture newTex);
	
	//  Retrieve the list of textures within the material
	std::vector<Texture> GetTextures();
	//  Retrieve the list of shaders used by the material
	std::vector<Shader> GetShaders();

private:

	//  Texture objects
	std::vector<Texture> textures;

	//  Storage for all shader components
	std::vector<Shader> shaders;

};

