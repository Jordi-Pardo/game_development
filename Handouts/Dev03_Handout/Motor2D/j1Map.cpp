#include "p2Defs.h"
#include "p2Log.h"
#include "j1App.h"
#include "j1Render.h"
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


	map_file.reset();

	return true;
}

// Load new map
bool j1Map::Load(const char* file_name)
{
	bool ret = true;
	p2SString tmp("%s%s", folder.GetString(), file_name);

	pugi::xml_parse_result result = map_file.load_file(tmp.GetString());

	if(result == NULL)
	{
		LOG("Could not load map xml file %s. pugi error: %s", file_name, result.description());
		ret = false;
	}

	if(ret == true)
	{
		// TODO 3: Create and call a private function to load and fill
		// all your map data
		LoadMap(map_file.child("map"));
	}

	// TODO 4: Create and call a private function to load a tileset
	// remember to support more any number of tilesets!
	LoadTileset(map_file.child("map").child("tileset"));
	

	if(ret == true)
	{
		// TODO 5: LOG all the data loaded
		// iterate all tilesets and LOG everything
	}

	map_loaded = ret;

	return ret;
}

bool j1Map::LoadMap(pugi::xml_node& map)
{
	p2SString orientation = "orthogonal";
	bool match = strcmp(orientation.GetString(), map.attribute("orientation").as_string());
	if (match) {
		mapNode.orientation = Orientation::OR_ORTHOGONAL;
	}
	p2SString render = "right-down";
	bool match = strcmp(orientation.GetString(), map.attribute("renderorder").as_string());
	if (match) {
		mapNode.renderorder = Renderorder::REN_RIGHT_DOWN;
	}
	mapNode.width = map.attribute("width").as_int();
	mapNode.height = map.attribute("height").as_int();
	mapNode.tilewidth = map.attribute("tilewidth").as_int();
	mapNode.tileheight = map.attribute("tileheight").as_int();
	mapNode.nextobjectid = map.attribute("nextobjectid").as_int();
	
	return true;
}

bool j1Map::LoadTileset(pugi::xml_node& tile)
{
	
	p2SString name = tile.attribute("name").as_string();
	tileSet.firstgid = tile.attribute("firstgid").as_int();
	tileSet.tilewidth = tile.attribute("tilewidth").as_int();
	tileSet.tileheigth = tile.attribute("tileheigth").as_int();
	tileSet.spacing = tile.attribute("spacing").as_int();
	tileSet.margin = tile.attribute("margin").as_int();

	p2SString image_source = tile.attribute("image_source").as_string();
	uint image_width = tile.attribute("image_width").as_int();
	uint image_height = tile.attribute("image_height").as_int();

	return true;
}

