#pragma once

struct collision_bsp;
struct large_collision_bsp;
struct c_bsp3d_reference
{
public:
	//c_bsp3d_reference();

protected:
	collision_bsp* m_bsp;
	large_collision_bsp* m_large_bsp;
};

struct structure_bsp;
struct s_structure_bsp_resources;
struct c_collision_bsp_reference : public c_bsp3d_reference
{
public:
	//c_collision_bsp_reference();
	//c_collision_bsp_reference(structure_bsp const* bsp);
};