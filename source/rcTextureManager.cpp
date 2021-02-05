//includes
#include <stdafx.h>
#include "rcTexture.h"
#include "rcTextureManager.h"
//constructor initalising texture map
TextureManager::TextureManager(): m_pTextureMap()
{}
//deconstructor clearing texture map
TextureManager::~TextureManager()
{
	m_pTextureMap.clear();
}
//setting class pointer to null
TextureManager* TextureManager::m_instance = nullptr;

Texture* TextureManager::LoadTexture(const char* a_fileName, u32 a_format)
{
	Texture* pTexture = nullptr;
	if (a_fileName != nullptr) //if not null
	{
		std::map<std::string, TextureRef>::iterator dictionaryIter =
			m_pTextureMap.find(a_fileName); //dictonary iterate through texture map to find the parameter
		if (dictionaryIter != m_pTextureMap.end()) //if not at the end then located
		{
			TextureRef& textRef = (TextureRef&)(dictionaryIter->second);
			//reference to texture ref struct
			pTexture = textRef.pTexture;
			++textRef.refCount;
		}
		else
		{
			pTexture = new Texture(); //class instance
			if (pTexture->Load(a_fileName, a_format)) //load texture 
			{//place into text ref struct and insert into dictionary
				TextureRef textRef = {pTexture, 1};
				m_pTextureMap[a_fileName] = textRef;
			}
			else { delete pTexture; pTexture = nullptr; } //load failed free memory and reset to null
		}
	}
	return pTexture;
}

void TextureManager::ReleaseTexture(Texture* a_pTexture)
{
	for (auto dictionaryIter = m_pTextureMap.begin();
		dictionaryIter != m_pTextureMap.end(); ++dictionaryIter) //loop through dictionary from texture map start to end
	{
		TextureRef& textRef = (TextureRef&)(dictionaryIter->second);
		if (a_pTexture == textRef.pTexture) //found tex to remove
		{
			if (--textRef.refCount == 0)
			{//start removing texture
				delete textRef.pTexture;
				textRef.pTexture = nullptr;
				m_pTextureMap.erase(dictionaryIter);
				break;
			}
		}
	}
}

TextureManager* TextureManager::CreateInstance()
{
	if (m_instance == nullptr) //if null
	{
		m_instance = new TextureManager(); //new class instance
	}
	return m_instance;
}

void TextureManager::DestroyInstance()
{
	if (m_instance != nullptr) //if not null
	{
		delete m_instance; //delete class instance
		m_instance = nullptr; //set to null
	}
}

bool TextureManager::TextureExists(const char* a_fileName)
{
	auto dictionaryIter = m_pTextureMap.find(a_fileName); //use dictonary to find file parameter
	if (dictionaryIter != m_pTextureMap.end()) //if not at end
	{
		return true;
	}
	return false;
}
//return a pointer to the texture and incremente the reference count
Texture* TextureManager::GetTexture(const char* a_fileName)
{
	Texture* pTexture = nullptr;
	auto dictionaryIter = m_pTextureMap.find(a_fileName);
	if (dictionaryIter != m_pTextureMap.end())
	{//if iterator is not at end then texture was located 
		TextureRef& textRef = (TextureRef&)(dictionaryIter->second);
		//get reference to texture ref struct
		textRef.refCount++;
		pTexture = textRef.pTexture;
	}
	return pTexture;
}