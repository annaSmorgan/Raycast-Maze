#pragma once
#ifndef  _LEVEL_
#define _LEVEL_
//includes
#include <stdafx.h>
//included classes
class Player;
class TextureManager;
class Texture;

class Level
{
public:
	Level(); //constructor
	~Level(); //deconstructor
	//getter functions
	void GetSize(u32& a_w, u32& a_h);
	u8 GetGridValue(u32 a_x, u32 a_y);
	//loading and drawing functions
	bool LoadLevel(const char* a_filename);
	void Draw(const Player* a_player);

private:
	//Texture pointers for images used by level
	Texture* m_imageData;
	Texture* m_imageData2;

	//variables
	u32 m_width;
	u32 m_height;
	
	//level map
	u8* m_map;
		
};

#endif //! _LEVEL_