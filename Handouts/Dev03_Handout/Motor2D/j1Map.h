#ifndef __j1MAP_H__
#define __j1MAP_H__

#include "PugiXml/src/pugixml.hpp"
#include "p2List.h"
#include "p2Point.h"
#include "j1Module.h"

// TODO 2: Create a struct to hold information for a TileSet
// Ignore Terrain Types and Tile Types for now, but we want the image!
// ----------------------------------------------------
struct TileSet
{
	uint firstgid = 0u;
	p2SString name;
	uint tilewidth = 0u;
	uint tileheigth = 0u;
	uint spacing = 0u;
	uint margin = 0u;

	p2SString image_source;
	uint image_width = 0u;
	uint image_height = 0u;
};


// TODO 1: Create a struct needed to hold the information to Map node
enum Orientation
{
	OR_NULL,
	OR_ORTHOGONAL,
	OR_ISOMETRIC,
	OR_HEXAGONAL
};

enum Renderorder
{
	REN_NULL,
	REN_RIGHT_DOWN,
	REN_RIGHT_UP,
	REN_LEFT_DOWN,
	REN_LEFT_UP,
};
struct MapNode
{
	Orientation orientation = OR_NULL;
	Renderorder renderorder = REN_NULL;
	uint width = 0;
	uint height = 0;
	uint tilewidth = 0;
	uint tileheight = 0;
	uint nextobjectid = 0;
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
	bool LoadMap(pugi::xml_node& map);
	bool LoadTileset(pugi::xml_node& map);

public:

	// TODO 1: Add your struct for map info as public for now

private:

	pugi::xml_document	map_file;
	p2SString			folder;
	bool				map_loaded;
	MapNode				mapNode;
	TileSet				tileSet;
};

#endif // __j1MAP_H__