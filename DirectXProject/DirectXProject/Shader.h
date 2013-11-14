#pragma once
#include <string>


class Shader
{
public:
	enum ShaderType {VERTEX, PIXEL};

	Shader();
	Shader(std::string filePath, ShaderType type);

	void SetShader(std::string filePath, ShaderType type);
	std::string GetPath();
	ShaderType GetShaderType();
	void SetLoaded(bool isLoaded);
	bool IsLoaded();

private:
	ShaderType shaderType;
	std::string filePath;
	unsigned int rendererLookup;
	bool loaded;
};
