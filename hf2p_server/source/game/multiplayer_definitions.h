#pragma once
#include <tag_files\tag_block.h>

struct s_multiplayer_universal_globals_definition
{

};

struct s_multiplayer_runtime_globals_definition
{

};

struct s_multiplayer_globals_definition
{
	c_tag_block<s_multiplayer_universal_globals_definition> universal;
	c_tag_block<s_multiplayer_runtime_globals_definition> runtime;
};