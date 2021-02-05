#include <stdafx.h>
#pragma once
#ifndef _TEXTURE_MANAGER_H_
#define _TEXTURE_MANAGER_H_

class Texture;

class TextureManager
{
public:
	//Singleton accessor, retrives instance of class
	static TextureManager* GetInstance() { return m_instance; }
	//Singleton creation, called to created an instance of class and return it
	static TextureManager* CreateInstance();
	//destroy instance is used to remove instance of class
	static void DestroyInstance();

	bool TextureExists(const char* a_pTextureName);
	Texture* LoadTexture(const char* a_pTextureName, u32 a_format);
	Texture* GetTexture(const char* a_fileName);

	void ReleaseTexture(Texture* a_texturePointer);

private:
	//static instance of class
	static TextureManager* m_instance;
	//constructor and destructor are private so cannot be called outside of instance
	TextureManager();
	~TextureManager();

	typedef struct TextureRef
	{
		Texture* pTexture;
			unsigned int refCount;
	}TextureRef;
	std::map<std::string, TextureRef> m_pTextureMap;
};

#endif // !_TEXTURE_MANAGER_H_

