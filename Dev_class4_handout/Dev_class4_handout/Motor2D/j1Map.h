#ifndef __j1MAP_H__
#define __j1MAP_H__

#include "PugiXml/src/pugixml.hpp"
#include "p2List.h"
#include "p2Point.h"
#include "j1Module.h"
#include "p2DynArray.h"
// TODO 2: Create a struct to hold information for a TileSet
// Ignore Terrain Types and Tile Types for now, but we want the image!


// TODO 1: Create a struct needed to hold the information to Map node

enum orientation 
{
	ORTHOGONAL,
	ISOMETRIC,
};

enum renderOrder
{
	RIGHTDOWN,
	LEFTDOWN,
	RIGHTUP,
	LEFTUP
};

struct tileOffset
{
	int x;
	int y;
};


struct MapNode
{
	int width;
	int height;
	int tileWidth;
	int tileHeigth;
	int nextObjectId;

	

	tileOffset Offset;
	renderOrder render;
	orientation orient;

	TileSet tiles;

	MapNode()
	{
		width = 0;
		height = 0;
		tileWidth = 0;
		tileHeigth = 0;
		nextObjectId = 0;

		Offset.x = 0;
		Offset.y = 0;
		render = RIGHTDOWN;
		orient = ORTHOGONAL;
	}
};


struct TileSet
{
	int firstGid;
	int tileWidth;
	int tileHeight;
	int spacing;
	int margin;

	p2DynArray<int> tileGrid;

	TileSet()
	{
		firstGid = 0;
		tileWidth = 0;
		tileHeight = 0;
		spacing = 0;
		margin = 0;
	}
};


// ----------------------------------------------------
class j1Map : public j1Module
{
public:

	j1Map();

	// Destructor
	virtual ~j1Map();

	// Called before render is available
	bool Awake(pugi::xml_node& conf);

	// Called each loop iteration
	void Draw();

	// Called before quitting
	bool CleanUp();

	// Load new map
	bool Load(const char* path);

private:

public:

	
	MapNode Level1;

private:

	pugi::xml_document	map_file;
	p2SString			folder;
	bool				map_loaded;
	MapNode				map;
};

#endif // __j1MAP_H__