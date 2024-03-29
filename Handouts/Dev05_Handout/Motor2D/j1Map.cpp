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
	maxFrames = 5;
	currentFrame = 0;
	LOG("Loading Map Parser");
	bool ret = true;

	folder.create(config.child("folder").child_value());

	return ret;
}

void j1Map::Draw()
{
	if(map_loaded == false)
		return;

	// TODO 5(old): Prepare the loop to draw all tilesets + Blit
	 // for now we just use the first layer and tileset
	lay = data.layers.start;
	MapLayer* layer = lay->data;
	tile = data.tilesets.start;
	TileSet* tileset = tile->data;
	
	
	for (int t = 0; t < data.layers.count(); t++)
	{	
		for (uint i = 0; i < layer->num_tile_height; i++)
		{
			for (uint j = 0; j < layer->num_tile_width; j++)
			{
				int n = layer->Get(j, i);

				if (layer->data[n] != 0)
				{	
<<<<<<< HEAD
					if (layer->data[n] > array_Tileset[0].firstgid && layer->data[n] < array_Tileset[1].firstgid)
					{
						num = 0;
					}
					 /* if (layer->data[n] > array_Tileset[1].firstgid && layer->data[n] < array_Tileset[2].firstgid) {
						num = 1;
					}*/
					else if (layer->data[n] > array_Tileset[2].firstgid && layer->data[n] < array_Tileset[3].firstgid) {
						num = 2;
					}
					else if(layer->data[n] >= array_Tileset[3].firstgid) {
						num = 3;
					}
						App->render->Blit(array_Tileset[num].texture, MapToWorld(j, i).x, MapToWorld(j, i).y, &GetRect(&array_Tileset[num], layer->data[n]),3.0f);
=======
					if (layer->data[n] < tile->next->data->firstgid && layer->data[n] > tile->data->firstgid) 
					{

						
					}
						App->render->Blit(tileset->texture, MapToWorld(j, i).x, MapToWorld(j, i).y, &GetRect(tileset, layer->data[n]));
>>>>>>> parent of 4aba902... Tried to load more tilesets
				}
			}

		}
		
		

		
		if (lay->next != nullptr) {
			lay = lay->next;
			layer = lay->data;
		}
	
		//if (currentFrame == maxFrames) {
		//	if (lay->next != nullptr) {
		//		lay = lay->next;
		//		layer = lay->data;
		//		currentFrame = 0;
		//	}
		//	else {
		//		lay = data.layers.start;
		//		currentFrame = 0;
		//	}
		//}else{
		//	currentFrame++;
		//}
		
	}
	//lay = data.layers.start;

	// TODO 10(old): Complete the draw function
}

iPoint j1Map::MapToWorld(int x, int y) const
{
	iPoint ret(0,0);
	// TODO 8(old): Create a method that translates x,y coordinates from map positions to world positions

	// TODO 1: Add isometric map to world coordinates
	switch (data.type)
	{
	case MapTypes::MAPTYPE_ORTHOGONAL:
		ret.x = x * data.tile_width;
		ret.y = y * data.tile_height;
		break;
	case MapTypes::MAPTYPE_ISOMETRIC:
		ret.x = (x - y) * data.tile_width / 2;
		ret.y = (x + y) * data.tile_height / 2;
	
		break;

	}
	return ret;
}


iPoint j1Map::WorldToMap(int x, int y) const
{
	iPoint ret(0,0);
	// TODO 2: Add orthographic world to map coordinates
	switch (data.type)
	{
	case MapTypes::MAPTYPE_ORTHOGONAL:
		ret.x = x / data.tile_width;
		ret.y = y / data.tile_height;
		break;
	case MapTypes::MAPTYPE_ISOMETRIC:
		ret.x = (x / data.tile_width / 2 + y / data.tile_height / 2) / 2;
		ret.y = (y / data.tile_height / 2 - (x / data.tile_width / 2)) / 2;
	default:
		break;
	}

	// TODO 3: Add the case for isometric maps to WorldToMap
	return ret;
}


// Called before quitting
bool j1Map::CleanUp()
{
	LOG("Unloading map");

	// Remove all tilesets
	p2List_item<TileSet*>* item;
	item = data.tilesets.start;

	while(item != NULL)
	{
		RELEASE(item->data);
		item = item->next;
	}
	data.tilesets.clear();

	// Remove all layers
	p2List_item<MapLayer*>* item2;
	item2 = data.layers.start;

	while(item2 != NULL)
	{
		RELEASE(item2->data);
		item2 = item2->next;
	}
	data.layers.clear();

	// Clean up the pugui tree
	map_file.reset();

	return true;
}

// Load new map
bool j1Map::Load(const char* file_name)
{
	bool ret = true;
	p2SString tmp("%s%s", folder.GetString(), file_name);

	pugi::xml_parse_result result = map_file.load_file(file_name);

	if(result == NULL)
	{
		LOG("Could not load map xml file %s. pugi error: %s", file_name, result.description());
		ret = false;
	}

	// Load general info ----------------------------------------------
	if(ret == true)
	{
		ret = LoadMap();
	}

	// Load all tilesets info ----------------------------------------------
	pugi::xml_node tileset;
	for(tileset = map_file.child("map").child("tileset"); tileset && ret; tileset = tileset.next_sibling("tileset"))
	{
		TileSet* set = new TileSet();

		if(ret == true)
		{
			ret = LoadTilesetDetails(tileset, set);
		}

		if(ret == true)
		{
			ret = LoadTilesetImage(tileset, set);
		}

		data.tilesets.add(set);
	}

	int num = data.tilesets.count();
	tile = data.tilesets.start;
	array_Tileset = new TileSet[num]();
	for (int i = 0; i < data.tilesets.count(); i++)
	{
		array_Tileset[i] = *tile->data;
		if(tile->next != nullptr)
		tile = tile->next;
	}
	tile = data.tilesets.start;
	for (int i = 0; i < 4; i++)
	{
		LOG("%d", array_Tileset[i].firstgid);
	}


	// Load layer info ----------------------------------------------
	pugi::xml_node layer;
	for(layer = map_file.child("map").child("layer"); layer && ret; layer = layer.next_sibling("layer"))
	{
		MapLayer* lay = new MapLayer();

		ret = LoadLayer(layer, lay);

		if(ret == true)
			data.layers.add(lay);
	}

	if(ret == true)
	{
		LOG("Successfully parsed map XML file: %s", file_name);
		LOG("width: %d height: %d", data.width, data.height);
		LOG("tile_width: %d tile_height: %d", data.tile_width, data.tile_height);

		p2List_item<TileSet*>* item = data.tilesets.start;
		while(item != NULL)
		{
			TileSet* s = item->data;
			LOG("Tileset ----");
			LOG("name: %s firstgid: %d", s->name.GetString(), s->firstgid);
			LOG("tile width: %d tile height: %d", s->tile_width, s->tile_height);
			LOG("spacing: %d margin: %d", s->spacing, s->margin);
			item = item->next;
		}

		p2List_item<MapLayer*>* item_layer = data.layers.start;
		while(item_layer != NULL)
		{
			MapLayer* l = item_layer->data;
			LOG("Layer ----");
			LOG("name: %s", l->name.GetString());
			LOG("tile width: %d tile height: %d", l->num_tile_width, l->num_tile_height);
			item_layer = item_layer->next;
		}
	}

	map_loaded = ret;

	return ret;
}

// Load map general properties
bool j1Map::LoadMap()
{
	bool ret = true;
	pugi::xml_node map = map_file.child("map");

	if(map == NULL)
	{
		LOG("Error parsing map xml file: Cannot find 'map' tag.");
		ret = false;
	}
	else
	{
		data.width = map.attribute("width").as_int();
		data.height = map.attribute("height").as_int();
		data.tile_width = map.attribute("tilewidth").as_int();
		data.tile_height = map.attribute("tileheight").as_int();
		p2SString bg_color(map.attribute("backgroundcolor").as_string());

		data.background_color.r = 0;
		data.background_color.g = 0;
		data.background_color.b = 0;
		data.background_color.a = 0;

		if(bg_color.Length() > 0)
		{
			p2SString red, green, blue;
			bg_color.SubString(1, 2, red);
			bg_color.SubString(3, 4, green);
			bg_color.SubString(5, 6, blue);

			int v = 0;

			sscanf_s(red.GetString(), "%x", &v);
			if(v >= 0 && v <= 255) data.background_color.r = v;

			sscanf_s(green.GetString(), "%x", &v);
			if(v >= 0 && v <= 255) data.background_color.g = v;

			sscanf_s(blue.GetString(), "%x", &v);
			if(v >= 0 && v <= 255) data.background_color.b = v;
		}

		p2SString orientation(map.attribute("orientation").as_string());

		if(orientation == "orthogonal")
		{
			data.type = MAPTYPE_ORTHOGONAL;
		}
		else if(orientation == "isometric")
		{
			data.type = MAPTYPE_ISOMETRIC;
		}
		else if(orientation == "staggered")
		{
			data.type = MAPTYPE_STAGGERED;
		}
		else
		{
			data.type = MAPTYPE_UNKNOWN;
		}
	}

	return ret;
}

bool j1Map::LoadTilesetDetails(pugi::xml_node& tileset_node, TileSet* set)
{
	bool ret = true;
	set->name.create(tileset_node.attribute("name").as_string());
	set->firstgid = tileset_node.attribute("firstgid").as_int();
	set->tile_width = tileset_node.attribute("tilewidth").as_int();
	set->tile_height = tileset_node.attribute("tileheight").as_int();
	set->margin = tileset_node.attribute("margin").as_int();
	set->spacing = tileset_node.attribute("spacing").as_int();
	pugi::xml_node properties = tileset_node.child("properties").child("property");
	if (strcmp(properties.attribute("name").as_string(), "player")== 0 && properties.attribute("value").as_bool() == true){
		set->isPlayer = true;
	}
	else {
		set->isPlayer = false;
	}
	pugi::xml_node offset = tileset_node.child("tileoffset");

	if(offset != NULL)
	{
		set->offset_x = offset.attribute("x").as_int();
		set->offset_y = offset.attribute("y").as_int();
	}
	else
	{
		set->offset_x = 0;
		set->offset_y = 0;
	}

	return ret;
}

bool j1Map::LoadTilesetImage(pugi::xml_node& tileset_node, TileSet* set)
{
	bool ret = true;
	pugi::xml_node image = tileset_node.child("image");

	if(image == NULL)
	{
		LOG("Error parsing tileset xml file: Cannot find 'image' tag.");
		ret = false;
	}
	else
	{
		set->texture = App->tex->Load(PATH(folder.GetString(), image.attribute("source").as_string()));
		int w, h;
		SDL_QueryTexture(set->texture, NULL, NULL, &w, &h);
		set->tex_width = image.attribute("width").as_int();

		if(set->tex_width <= 0)
		{
			set->tex_width = w;
		}

		set->tex_height = image.attribute("height").as_int();

		if(set->tex_height <= 0)
		{
			set->tex_height = h;
		}

		set->num_tiles_width = set->tex_width / set->tile_width;
		set->num_tiles_height = set->tex_height / set->tile_height;
	}

	return ret;
}

bool j1Map::LoadLayer(pugi::xml_node& node, MapLayer* layer)
{
	bool ret = true;

	layer->name = node.attribute("name").as_string();
	layer->num_tile_width = node.attribute("width").as_int();
	layer->num_tile_height = node.attribute("height").as_int();
	pugi::xml_node layer_data = node.child("data");

	if(layer_data == NULL)
	{
		LOG("Error parsing map xml file: Cannot find 'layer/data' tag.");
		ret = false;
		RELEASE(layer);
	}
	else
	{
		layer->data = new uint[layer->num_tile_width*layer->num_tile_height];
		memset(layer->data, 0, layer->num_tile_width*layer->num_tile_height);

		int i = 0;
		for(pugi::xml_node tile = layer_data.child("tile"); tile; tile = tile.next_sibling("tile"))
		{
			layer->data[i++] = tile.attribute("gid").as_int(0);
		}
	}

	return ret;
}

SDL_Rect j1Map::GetRect(TileSet* tileset, int id)
{

	//----NO PERFORMANCE-----
	//int i = 0;
	//int j = 0;
	//int num = 0;
	//int target = id;
	//int width = 0;
	//int height = 0;
	//for (i = 0; i < data.tilesets[0]->num_tiles_height && num < target; i++)
	//{

	//	for (j = 0; j < data.tilesets[0]->num_tiles_width && num < target; j++)
	//	{
	//		num++;
	//	}
	//
	//}
	//width = (j - 1) * data.tile_width + j ;
	//height = (i - 1) * data.tile_width + i;
	//SDL_Rect rect = {width,height,32,32};

	//-----PERFORMANCE-----
	int num = id;

	int x = (num - tileset->firstgid) % tileset->num_tiles_width;
	int y = (num - tileset->firstgid) / tileset->num_tiles_width;
	int width = 0;
	int height = 0;
	if (tileset->isPlayer) {

		width = x * data.tile_width + tileset->margin;
		height = y * data.tile_width + tileset->spacing;
	}
	else {
		width = x * data.tile_width + (x+ tileset->margin) * tileset->margin;
		height = y * data.tile_width + (y+ tileset->spacing) * tileset->spacing;
	}

	SDL_Rect rect = { width,height,tileset->tile_width,tileset->tile_height };
	return	rect;
}