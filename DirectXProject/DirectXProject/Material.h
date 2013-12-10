#pragma once

#include "Texture.h"
#include "Component.h"

#include <map>

class Material : public Component<Material>
{
public:
	Material(){}
	~Material(){}


	template<class T> void AddTexture(std::string file) {
		TextureType type = T::GetTextureTypeID();
		ObjectID id = T::New(file);
		if (textures.count(type) == 0) {
			textures[type] = id;
		} else {
			//panic
		}
	}

	template<class T> void RemoveTexture() {
		TextureType type = T::GetTextureTypeID();
		ObjectID id = 0;
		if (textures.count(type) != 0) {
			id = textures[type];
			textures.erase(type);
			T::DeleteFromStorage(id);
		} else {
			//panic
		}			
	}

	template<class T> ID3D11ShaderResourceView* GetTextureResource() {
		TextureType type = T::GetTextureTypeID();
		ObjectID id = textures[type];
		return T::GetTexture(id);
	}

	template<class T> T& GetTexture() {
		TextureType type = T::GetTextureTypeID();
		ObjectID id = textures[type];
		return T::Get(id);
	}

private:
	std::map<ComponentType, ObjectID> textures;
};
