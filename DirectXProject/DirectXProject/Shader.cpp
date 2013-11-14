#include "Shader.h"


Shader::Shader()
{
}

Shader::Shader(std::string path, ShaderType type):
	filePath(path), 
	shaderType(type)
{
}

void Shader::SetShader(std::string path, ShaderType type)
{
	filePath = path;
	shaderType = type;
}


std::string Shader::GetPath()
{
	return filePath;
}

Shader::ShaderType Shader::GetShaderType()
{
	return shaderType;
}

void Shader::SetLoaded(bool isLoaded)
{
	loaded = isLoaded;
}

bool Shader::IsLoaded()
{
	return loaded;
}