#pragma once
#include <memory\data.h>
#include <game\materials.h>

template <typename t_type>
struct hkArrayBase
{
public:
	t_type* m_data;
	long m_size;
	dword m_capacityAndFlags;
};
static_assert(sizeof(hkArrayBase<void*>) == 0xC);

template <typename t_type, typename t_allocator = void>
struct hkArray :
	public hkArrayBase<t_type>
{
public:
};
static_assert(sizeof(hkArray<void*>) == 0xC);

struct c_havok_contact_point
{
	dword m_environment_shape_key;
	long m_impact_datum_index;
	long m_contact_surface_index;
	c_global_material_type m_global_material_type_a;
	c_global_material_type m_global_material_type_b;
	long m_contact_havok_component_index;
	long m_contact_point_object_index;
	real_point3d m_position;
	real_vector3d m_normal;
	real m_distance;
	real m_friction;
	real m_restitution;
	word m_contact_point_id;

	// FLAG(0) _new_bit
	// FLAG(1) _deleted_bit
	word_flags m_life_cycle_flags;

	// FLAG(1) _physics_only_surface_bit
	// FLAG(2) _climbable_surface_bit
	// FLAG(3) _breakable_surface_bit
	// FLAG(6) _slip_surface_bit
	byte_flags m_flags;

	char m_rigid_body_index_a;
	char m_rigid_body_index_b;
	char m_structure_bsp_index;
};
static_assert(sizeof(c_havok_contact_point) == 0x44);

struct hkRigidBody;
struct hkConstraintInstance;
struct hkWorldObject;
class c_havok_component : s_datum_header
{
public:
	void force_activate(bool active);

private:
	struct c_rigid_body
	{
		struct s_contact_point_buffer
		{
			c_havok_contact_point contact_point;
		};
		static_assert(sizeof(s_contact_point_buffer) == 0x44);

		hkArray<s_contact_point_buffer> m_havok_contact_points;
		hkArray<char> m_physics_model_rigid_body_indexes;

		byte __dataC[0x4];

		real_point3d m_pre_simulation_position;
		real_vector3d m_pre_simulation_linear_velocity;
		real_vector3d m_pre_simulation_angular_velocity;
		hkRigidBody* m_rigid_body;

		// FLAG(0) _plays_impact_effects_bit
		// FLAG(1) _has_infinite_inertia_tensor_bit
		// FLAG(2) _get_does_not_collide_with_environment_bit
		byte_flags m_flags;
	};
	static_assert(sizeof(c_rigid_body) == 0x48);

	struct c_constraint
	{
		// FLAG(0) powered_bit
		byte_flags m_flags;

		char m_constraint_type;
		char m_constraint_type_index;
		char m_stretched_counter;
		long m_constraint_impact_datum_index;
		hkConstraintInstance* m_constraint;
	};
	static_assert(sizeof(c_constraint) == 0xC);

	struct s_body_in_phantom
	{
		char body_physics_model_material_index;
		char body_rigid_body_index;
		long body_object_index;
		long reference_count;
	};
	static_assert(sizeof(s_body_in_phantom) == 0xC);

	// FLAG(0) _records_contact_points_bit
	// FLAG(1) _ignores_gravity_bit
	// FLAG(2) _connected_to_world_bit
	// FLAG(3) _keyframed_bit
	// FLAG(4) _sentinal_bit
	// FLAG(5) _one_body_object_space_component_bit
	// FLAG(6) _constrained_to_world_bit
	// FLAG(7) _physics_model_havok_component_bit
	dword_flags m_flags;

	long m_object_index;
	byte m_object_postion_controlling_rigid_body_index;
	byte m_object_postion_controlling_node_index;
	byte m_maximum_rigid_body_size;
	long m_impact_array_index;
	hkArray<c_rigid_body> m_havok_rigid_bodies;
	c_rigid_body m_rigid_body;
	hkArray<c_constraint> m_constraints;
	hkArray<s_body_in_phantom>* m_bodies_in_phantom;
	hkWorldObject* m_simple_shape_phantom;
	hkWorldObject* m_aabb_phantom;
};
static_assert(sizeof(c_havok_component) == 0x80);

s_data_array* havok_components_get_data_array();