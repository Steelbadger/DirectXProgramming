#pragma once
#include "Texture.h"

class AmbientTexture : public Texture<AmbientTexture>
{
public:
	AmbientTexture(){}
	~AmbientTexture(){}
};


class SpecularMap : public Texture<SpecularMap>
{
public:
	SpecularMap(){}
	~SpecularMap(){}
};

class NormalMap : public Texture<NormalMap>
{
public:
	NormalMap(){}
	~NormalMap(){}
};

class DisplacementMap : public Texture<DisplacementMap>
{
public:
	DisplacementMap(){}
	~DisplacementMap(){}
	void SetDisplacementMagnitude(float mag){displacementMagnitude = mag;}
	float GetDisplacementMagnitude(){return displacementMagnitude;}
private:
	float displacementMagnitude; 
};
