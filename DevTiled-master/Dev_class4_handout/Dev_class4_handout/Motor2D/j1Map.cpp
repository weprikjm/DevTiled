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

}

// Called before quitting
bool j1Map::CleanUp()
{
	LOG("Unloading map");

	// TODO 2: Make sure you clean up any memory allocated
	// from tilesets / map

	// Remove all tilesets


	map_file.reset();

	return true;
}

// Load new map
bool j1Map::Load(const char* file_name)
{
	bool ret = true;
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
		// TODO 3: Create and call a private function to load and fill
		// all your map data
		map.height = map_file.child("map").find_attribute("height").as_int();
		map.width = map_file.child("map").find_attribute("width").as_int();
		map.tileWidth = map_file.child("map").find_attribute("tileWidth").as_int();
		map.tileHeigth = map_file.child("map").find_attribute("tileHeight").as_int();
		map.nextObjectId = map_file.child("map").find_attribute("nextObjectId").as_int();
		


		if (map_file.child("map").find_attribute("orientation").as_string() == "ORTHOGONAL")
		{
			map.orient = ORTHOGONAL;
		}
		
		if (map_file.child("map").find_attribute("orientation").as_string() == "right-down")
		{
			map.render = RIGHTDOWN;
		}
		 
	}

	// TODO 4: Create and call a private function to load a tileset
	// remember to support more any number of tilesets!

	for (pugi::xml_node nTMP = map_file.child("map").child("tileset"); nTMP; nTMP = )

	map.tiles.add()

	map.tiles.firstGid = map_file.child("map").child("tileset").attribute("firstgid").as_int();
	map.tiles.margin = map_file.child("map").child("tileset").attribute("margin").as_int();
	map.tiles.spacing = map_file.child("map").child("tileset").attribute("spacing").as_int();
	map.tiles.tileHeight = map_file.child("map").child("tileset").attribute("tileheight").as_int();
	map.tiles.tileWidth = map_file.child("map").child("tileset").attribute("tilewidth").as_int();

	

	for (pugi::xml_node nTMP = map_file.child("map").child("tileset").child("tile"); nTMP ; nTMP = nTMP.next_sibling("tile"))
	{
	
		map.tiles.tileGrid.PushBack(nTMP.attribute("id").as_int());
	
	}


	map.tiles.tileGrid.Count();






	// TODO 5: LOG all the data loaded
	// iterate all tilesets and LOG everything

	map_loaded = ret;

	return ret;
}
