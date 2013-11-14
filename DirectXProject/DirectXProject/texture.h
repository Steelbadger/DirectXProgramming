#pragma once
///////////////////////// By Ross Davies /////////////////////////
//	The texture class represents a single texture, this class	//
//	handles loading textures from files or arrays into the		//
//  OpenGL context												//
//////////////////////////////////////////////////////////////////

#include <string>
#include <map>


class Texture
{
public:
	//  Textures must be one of 4 types
	enum Type {DIFFUSE, NORMAL, DISPLACEMENT, SPECULAR};

	//  Constructors, texture can be loaded from string(file) or an array
	Texture(void);
	Texture(Type t, std::string path);

	~Texture(void);

	//  Set the image associated with the texture object, can be loaded from
	//  a string (file) or an array.  Special case for handling a displacement
	//  map as this requires a magnitude for the displacements
	void SetTexture(Type t, std::string path);
	void SetLoaded(bool isLoaded);
	bool IsLoaded();

	Type GetType(){return type;}

private:
	Type type;

	//  The file path
	std::string path;

	bool loaded;
};

