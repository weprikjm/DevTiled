#include "p2Defs.h"
#include "p2Log.h"
#include "j1App.h"
#include "j1Render.h"
#include "j1FileSystem.h"
#include "j1Textures.h"
#include "j1Map.h"
#include <math.h>

j1Map::j1Map() : j1Module(), map_loaded(false)
{
	name.create("map");
}

// Destructor
j1Map::~j1Map()
{}

// Called before render is available
bool j1Map::Awake(pugi::xml_node& config)
{
	LOG("Loading Map Parser");
	bool ret = true;

	folder.create(config.child("folder").child_value());

	


	return ret;
}

void j1Map::Draw()
{
	if(map_loaded == false)
		return;

	// TODO 6: Iterate all tilesets and draw all their 
	// images in 0,0 (you should have only one tileset for now)
	
	
	
	
	SDL_Texture* gameArtTiles = NULL;
	gameArtTiles = LoadRootTileGraphics(gameArtTiles);

	

	for (int i = 0; i < map.numTileSets; i++)
	{ 
		p2List_item<TileSet*>* tmpTileSet = map.tiles.At(i);
		for (int i = 0; i < map.tiles.count();i++)
		{ 
			App->render->Blit(gameArtTiles, 0, 0);
		}
	}
		

	for (int i = 0; i < map.numLayers; i++)
	{
		for (int j = 0; j < map.height; j++)
		{
			for (int k = 0; k < map.width ; k++)
			{
				App->render->Blit(gameArtTiles, k*map.tileWidth, j*map.tileHeigth, &GetTileRect(Get(k,j)));
			}
		}
	
	}







}

// Called before quitting
bool j1Map::CleanUp()
{
	LOG("Unloading map");

	// TODO 2: Make sure you clean up any memory allocated
	// from tilesets / map
	
	
	p2List_item<TileSet*>* itemTileSet;
	itemTileSet = map.tiles.start;

	while (itemTileSet != NULL)
	{
		RELEASE(itemTileSet->data);
		itemTileSet = itemTileSet->next;
	}
	
	/*
	p2List_item<mapLayer*>* itemLayer;
	itemLayer = map.layers.start;

	while (itemLayer != NULL)
	{
		RELEASE(itemLayer->data);
		itemLayer = itemLayer->next;
	}
	*/

	//map.tiles.clear();
	// Remove all tilesets

	//map_file.reset();

	return true;
}

// Load new map
bool j1Map::Load(const char* file_name)
{
	bool ret = true;
	int numTileSets = 0;
	p2SString tmp("%s%s", folder.GetString(), file_name);

	char* buf;
	int size = App->fs->Load(tmp.GetString(), &buf);
	pugi::xml_parse_result result = map_file.load_buffer(buf, size);

	RELEASE(buf);

	if(result == NULL)
	{
		LOG("Could not load map xml file %s. pugi error: %s", file_name, result.description());
		ret = false;
	}

	if(ret == true)
	{
		
		numTileSets = LoadMapInfo();// TODO 3: Create and call a private function to load and fill all your map data
		
	}
	
	if (numTileSets > 0)	
		loadTileSet(numTileSets);// TODO 4: Create and call a private function to load a tileset

	if (map.numLayers > 0)
		LoadLayer(map.numLayers);//Fills maplayer with its information




	map_loaded = ret;

	return ret;
}


int j1Map::LoadMapInfo()
{
	//Set consistency later
		map.height = map_file.child("map").attribute("height").as_int();
		map.width = map_file.child("map").attribute("width").as_int();
		map.tileWidth = map_file.child("map").attribute("tileWidth").as_int();
		map.tileHeigth = map_file.child("map").attribute("tileHeight").as_int();
		map.nextObjectId = map_file.child("map").attribute("nextObjectId").as_int();

		if (map_file.child("map").attribute("orientation").as_string() == "ORTHOGONAL")
		{
			map.orient = ORTHOGONAL;
		}

		if (map_file.child("map").attribute("renderorder").as_string() == "right-down")
		{
			map.render = RIGHTDOWN;
		}

		map.numTileSets = map_file.child("map").attribute("numTileSets").as_int();
		map.numLayers = map_file.child("map").attribute("numLayers").as_int();

	return map.numTileSets;
}


bool j1Map::loadTileSet(int nTileSets)
{
	bool ret = false;


	for (int i = 0; i < nTileSets; i++)
	{
	
	
		map.tiles.add(new TileSet);

		p2List_item<TileSet*>* tileSetConfig = map.tiles.At(i);

		tileSetConfig->data->firstGid = map_file.child("map").child("tileset").attribute("firstgid").as_int();
		tileSetConfig->data->margin = map_file.child("map").child("tileset").attribute("margin").as_int();
		tileSetConfig->data->spacing = map_file.child("map").child("tileset").attribute("spacing").as_int();
		tileSetConfig->data->tileHeight = map_file.child("map").child("tileset").attribute("tileheight").as_int();
		tileSetConfig->data->tileWidth = map_file.child("map").child("tileset").attribute("tilewidth").as_int();



		for (pugi::xml_node nTMP = map_file.child("map").child("tileset").child("tile"); nTMP; nTMP = nTMP.next_sibling())
		{
			tileSetConfig->data->tileGrid.PushBack(nTMP.attribute("id").as_int());
		}


		for (int i = 0; i < tileSetConfig->data->tileGrid.Count(); i++)
		{
			int p = *tileSetConfig->data->tileGrid.At(i);
			LOG("%d", p);
		}


		

	}

	return ret;
}

SDL_Texture* j1Map::LoadRootTileGraphics(SDL_Texture* gameArtTiles)
{
	gameArtTiles = App->tex->Load(map_file.child("map").child("tileset").child("image").attribute("source").as_string());

	return gameArtTiles;
}


bool j1Map::LoadLayer(int nLayers)
{
	bool ret = false;


	for (int i = 0; i < nLayers; i++)
	{


		map.layers.add(new mapLayer);

		p2List_item<mapLayer*>* mapLayerConfig = map.layers.At(i);

		mapLayerConfig->data->height = map_file.child("map").child("layer").attribute("height").as_int();
		mapLayerConfig->data->width = map_file.child("map").child("layer").attribute("width").as_int();
		
		//child("data").child("tile").attribute("gid").as_int()


		for (pugi::xml_node nTMP = map_file.child("map").child("layer").child("data").child("tile"); nTMP; nTMP = nTMP.next_sibling())
		{
			mapLayerConfig->data->tileGuideline.PushBack(nTMP.attribute("gid").as_int());
		}


		for (int i = 0; i < mapLayerConfig->data->tileGuideline.Count(); i++)
		{
			int p = *mapLayerConfig->data->tileGuideline.At(i);
			LOG("%d", p);
		}

	}

	return ret;


}

MapNode j1Map::GetMapNode()
{
	return map;
}



SDL_Rect j1Map::GetTileRect(int id)
{
	int relative_id = id - map.tiles.At(0)->data->firstGid;
	p2List_item<TileSet*>* tile = map.tiles.At(0);
	

	SDL_Rect positionSprite;
	positionSprite.h = App->map->GetMapNode().height;
	positionSprite.w = App->map->GetMapNode().width;
	int margin = App->map->map.tiles.At(0)->data->margin;
	int spacing = App->map->map.tiles.At(0)->data->spacing;
	int num_tiles_width = App->map->map.tiles.At(0)->data->tileWidth;
	int num_tiles_heigth = App->map->map.tiles.At(0)->data->tileHeight;


	positionSprite.x = margin + ((positionSprite.w + spacing) * (relative_id % num_tiles_width));
	positionSprite.y = margin + ((positionSprite.h + spacing) * (relative_id / num_tiles_width));



	return positionSprite;
}

uint j1Map::Get(int x, int y)
{
	int width = App->map->map.layers.At(0)->data->width;

	return (uint)App->map->map.layers.At(0)->data->tileGuideline.At((y*width) + x);
}