//includes
#include <stdafx.h>
#include "renderer.h"
#include "player.h"
#include "Level.h"
#include "rcTextureManager.h"
#include "rcTexture.h"
#include "rcImageLoader.h"
//constructor initalising variables to 0 or null pointer
Level::Level(): m_width(0), m_height(0), m_map(nullptr), m_imageData(nullptr), m_imageData2(nullptr)
{																		  

}
//deconstructor
Level::~Level()
{
}
//gets the width and height
void Level::GetSize(u32& a_w, u32& a_h)
{
	a_w = m_width;
	a_h = m_height;
}
//gets the value on a grid
u8 Level::GetGridValue(u32 a_x, u32 a_y)
{
	u32 index = a_x + (a_y * m_width);
	if (index < (m_width * m_height)) //if smaller than map
	{
		return m_map[index];
	}
	return 255; // would indicate map out of bounds
}

bool Level::LoadLevel(const char* a_filename)
{
	if (m_map != nullptr) //if map is not null
	{
		delete[] m_map; //delete map
		m_map = nullptr; //set to null
	}
	std::fstream file; //new fstream
	file.open(a_filename, std::ios_base::in | std::ios_base::binary); //opening map file

	if (file.is_open()) //if its open correctly
	{
		file.ignore(std::numeric_limits<std::streamsize>::max()); //ignores limits and max
		std::streamsize fileSize = file.gcount(); //creates the file size as a variable
		file.clear();//clear error flags from being read
		file.seekg(0, std::ios_base::beg);//set to begining
		if (fileSize == 0) //if length 0
		{
			file.close(); //close file
			return false; //end 
		}
		//read in width and height
		file >> m_width >> m_height;
		file.ignore(1); //ignore end of line marker
		//allocate enough memory to store map
		m_map = new u8[m_width * m_height];
		u32 index = 0;
		//loop through each line in file
		for (std::string currentMapLine; std::getline(file, currentMapLine);)
		{//change get line to string stream and storing chunks as values in map
			std::istringstream ss{ currentMapLine };
			int val = 0;
			while (ss >> val)//whilst still have number data to read
			{
				m_map[index] = (u8)(val); //cast to u8
				++index; //increase index
				if ((ss >> std::ws).peek() == ',') //ignore white space and comma
				{
					ss.ignore();
				}
			}
		}
		//file close
		file.close();
		//instances of texture manager and loading both wall texture files
		TextureManager::GetInstance()->LoadTexture("../resources/images/wallTex1.pcx", rc_imageType::IM_PCX );
		TextureManager::GetInstance()->LoadTexture("../resources/images/wallTex2.pcx", rc_imageType::IM_PCX);
		//setting the texture to variable
		m_imageData = TextureManager::GetInstance()->GetTexture("../resources/images/wallTex1.pcx");
		m_imageData2 = TextureManager::GetInstance()->GetTexture("../resources/images/wallTex2.pcx");

		return true;//end succesfully
	}
	return false;//ends unsuccesfully
}

void Level::Draw(const Player* a_player)
{ 
	//instances
	renderer* render = renderer::GetInstance();
		
	//renderer's dimensions of screen
	u32 windowWidth = 0, windowHeight = 0;
	render->GetWindowSize(windowWidth, windowHeight);
	//pointer to get current postion and direction of player
	float playerPosX = 0.0f, playerPosY = 0.0f;
	a_player->GetPosition(playerPosX, playerPosY);
	float playerDirX = 0.0f, playerDirY = 0.0f;
	a_player->GetRotation(playerDirX, playerDirY);
	float nearPlaneLength = a_player->GetNearPlaneLength();

	//variables 
	u32 startDraw;
	u32 endDraw;
	s32 lineHeight;
	//texture dimensions
	u32 texWidth;	u32 texHeight;
	u32 tex2Width;	u32 tex2Height;
	//first texture
	double increase;
	double texturePos;
	//second texture
	double increase2;
	double texturePos2;
	//texture data
	m_imageData->GetDimensions(texWidth, texHeight);
	m_imageData2->GetDimensions(tex2Width, tex2Height);

	//camera
	float camPlaneX = -playerDirY * nearPlaneLength;
	float camPlaneY = playerDirX * nearPlaneLength;

	for (u32 c = 0; c < windowWidth; c++) //loop screen coloumns
	{
		//getting x as a value between -1 and 1
		float cameraX = ((2.0f * c) / (float)windowHeight) - 1.0f;
		float rayDirX = playerDirX + (camPlaneX * cameraX);
		float rayDirY = playerDirY + (camPlaneY * cameraX);

		//length of ray from x/y side to the next x/y side
		float deltaDistX = (rayDirX != 0.0f) ? std::abs(1.0f / rayDirX) : 0;
		float deltaDistY = (rayDirY != 0.0f) ? std::abs(1.0f / rayDirY) : 0;

		//players current grid pos
		int mapX = (int)(playerPosX);
		int mapY = (int)(playerPosY);

		//ray length from current postion to next x/y
		float sideDistX = 0.0f;
		float sideDistY = 0.0f;

		float perpWallDist = 0.0f;

		//direction to step in x/y +-1
		int stepX = 0;
		int stepY = 0;

		int collision = 0;
		if (rayDirX < 0) //where to start testing wall collisions based on X direction
		{
			stepX = -1;
			sideDistX = (playerPosX - mapX) * deltaDistX;
		}
		else
		{
			stepX = 1;
			sideDistX = (mapX + 1.0f - playerPosX) * deltaDistX;
		}

		if (rayDirY < 0)//where to start testing wall collisions based on Y direction
		{
			stepY = -1;
			sideDistY = (playerPosY - mapY) * deltaDistY;
		}
		else
		{
			stepY = 1;
			sideDistY = (mapY + 1.0f - playerPosY) * deltaDistY;
		}

		//tracks whether X or Y facing wall was hit
		int yIntersection = 0;
		while (collision == 0) //loop through ray cast testing whether collision is made
		{
			//jump to next map sqaure in x or y direction
			if (sideDistX < sideDistY)
			{
				sideDistX += deltaDistX;
				mapX += stepX;
				yIntersection = 0;
			}
			else
			{
				sideDistY += deltaDistY;
				mapY += stepY;
				yIntersection = 1;
			}
			collision = m_map[(mapX + (mapY * m_width))]; //test map location for collision
		}
		//once collided out ray
		perpWallDist = (!yIntersection) ?
			(mapX - playerPosX + (1 - stepX) / 2.0f) / rayDirX :
			(mapY - playerPosY + (1 - stepY) / 2.0f) / rayDirY;
		//calculate heigh of line to draw
		lineHeight = (int)(windowHeight / perpWallDist);
		//calc upper and lower points to draw
		s32 yPos = (windowHeight >> 1) - (lineHeight >> 1);
		yPos = (yPos < 0) ? 0 : yPos;

		startDraw = (u32)yPos; //position to start drawing
		endDraw = lineHeight / 2 + windowHeight / 2; //position to end drawing
		if (endDraw >= windowHeight) //if height is below or equal to end
		{
			endDraw = windowHeight - 1; //change end
		}

		double wallHitPoint = 0.0;
		if (yIntersection == 0) //work out whether wall hit a X or Y facing wall and set hit point to pos and dir
		{
			wallHitPoint = (double)(playerPosY + perpWallDist * rayDirY);
		}
		else
		{
			wallHitPoint = (double)(playerPosX + perpWallDist * rayDirX);
		}
		wallHitPoint -= floor((wallHitPoint)); //rounds to nearest whole number

		int texX = int(wallHitPoint * double(texWidth)); //texture X pos is hit point and width
		int texX2 = int(wallHitPoint * double(tex2Width));
		//check whether X or Y facing wall and if direction is above or below 0
		//setting the tex X pos to its width minus itself minus one 
		if (yIntersection == 0 && rayDirX > 0) 
		{
			texX = texWidth - texX - 1;
			texX2 = tex2Width - texX2 - 1;
		}
		if (yIntersection == 1 && rayDirY < 0)
		{
			texX = texWidth - texX - 1;
			texX2 = tex2Width - texX2 - 1;
		}
		//setting up the increase needed for the textures
		increase = 1.0 * texHeight / lineHeight;
		increase2 = 1.0 * tex2Height / lineHeight;
		//collecting the texture data from the texture variables
		u32* textureData = (u32*)m_imageData->GetPixelData();
		u32* textureData2 = (u32*)m_imageData2->GetPixelData();
		//setting the texture position to the start of the draw with the increase
		texturePos = (startDraw) * increase;
		texturePos2 = (startDraw ) * increase2;
		
		for (u32 y = startDraw; y < endDraw; y++)
		{//setting the texture Y by using the postion and height minus 1
			u32 texY = (int)texturePos & (texHeight - 1);
			u32 texY2 = (int)texturePos2 & (tex2Height - 1);
			//increasing the texture postion
			texturePos += increase;
			texturePos2 += increase2;

			u32 data = textureData[texWidth * texY + texX];
			u32 data2 = textureData2[tex2Width * texY2 + texX2];

			if (yIntersection == 1)//on Y facing walls
			{//adjust colours to appear darker
				data = (data >> 1) & 8355711;
				data2 = (data2 >> 1) & 8355711;
			}//send information to the buffer, texture on the wall depends whether it is Y or X facing
			render->FillRenderBuffer(c, y, 1, 1, (yIntersection) ? data : data2);
		}

	}
}
