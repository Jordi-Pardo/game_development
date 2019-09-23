// ----------------------------------------------------
// j1Module.h
// Interface for all engine modules
// ----------------------------------------------------

#ifndef __j1MODULE_H__
#define __j1MODULE_H__

#include "p2SString.h"
#include "PugiXml\src\pugixml.hpp"

class j1App;

class j1Module
{
public:

	j1Module() : active(false)
	{}

	void Init()
	{
		active = true;
	}

	// Called before render is available
	virtual bool Awake(pugi::xml_node * n)
	{		
		return true;
	}

	// Called before the first frame
	virtual bool Start()
	{
		return true;
	}

	// Called each loop iteration
	virtual bool PreUpdate()
	{
		return true;
	}

	// Called each loop iteration
	virtual bool Update(float dt)
	{
		return true;
	}

	// Called each loop iteration
	virtual bool PostUpdate()
	{
		return true;
	}

	// Called before quitting
	virtual bool CleanUp()
	{
		return true;
	}

	pugi::xml_node * SetNode(pugi::xml_node * n) {
		if (!n->empty()) {
			if (!n->child("modules").child(name.GetString()).empty())
			{
				LOG(name.GetString());
				return &n->child("modules").child(name.GetString());
			}
		}
		return nullptr;
	}

public:

	p2SString	name;
	bool		active;
	pugi::xml_node* node = nullptr;

};

#endif // __j1MODULE_H__