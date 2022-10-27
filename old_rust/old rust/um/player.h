class c_new_recoil_override
{
public:

	auto set_time_to_take_max( const float time ) -> void
	{
		driver->write<float>( reinterpret_cast< std::uintptr_t > ( this ) + m_time_to_take_max, time );
	}

	auto set_aimcone_curve_scale( const float scale ) -> void
	{
		driver->write<float>( reinterpret_cast< std::uintptr_t > ( this ) + m_aimcone_curve_scale, scale );
	}

	auto get_time_to_take_max() -> const float
	{
		return driver->read<float>( reinterpret_cast< std::uintptr_t > ( this ) + m_time_to_take_max );
	}

	auto get_aimcone_curve_scale( ) -> const float
	{
		return driver->read<float>( reinterpret_cast< std::uintptr_t > ( this ) + m_aimcone_curve_scale );
	}

	auto set_recoil_yaw_min( const float min ) -> void
	{
		driver->write<float>( reinterpret_cast< std::uintptr_t > ( this ) + m_recoil_yaw_min, min );
	}

	auto set_recoil_yaw_max( const float max ) -> void
	{
		driver->write<float>( reinterpret_cast< std::uintptr_t > ( this ) + m_recoil_yaw_max, max );
	}

	auto set_recoil_pitch_min( const float min ) -> void
	{
		driver->write<float>( reinterpret_cast< std::uintptr_t > ( this ) + m_recoil_pitch_min, min );
	}

	auto set_recoil_pitch_max( const float max ) -> void
	{
		driver->write<float>( reinterpret_cast< std::uintptr_t > ( this ) + m_recoil_pitch_max, max );
	}
};

class c_recoil_properties
{
public:

	auto set_time_to_take_max( const float time ) -> void
	{
		driver->write<float>( reinterpret_cast< std::uintptr_t > ( this ) + m_time_to_take_max, time );
	}

	auto set_aimcone_curve_scale( const float scale ) -> void
	{
		driver->write<float>( reinterpret_cast< std::uintptr_t > ( this ) + m_aimcone_curve_scale, scale );
	}

	auto set_recoil_yaw_min( const float min ) -> void
	{
		driver->write<float>( reinterpret_cast< std::uintptr_t > ( this ) + m_recoil_yaw_min, min );
	}

	auto set_recoil_yaw_max( const float max ) -> void
	{
		driver->write<float>( reinterpret_cast< std::uintptr_t > ( this ) + m_recoil_yaw_max, max );
	}

	auto set_recoil_pitch_min( const float min ) -> void
	{
		driver->write<float>( reinterpret_cast< std::uintptr_t > ( this ) + m_recoil_pitch_min, min );
	}

	auto set_recoil_pitch_max( const float max ) -> void
	{
		driver->write<float>( reinterpret_cast< std::uintptr_t > ( this ) + m_recoil_pitch_max, max );
	}

	auto get_new_recoil_override( ) -> c_new_recoil_override *
	{
		return driver->read<c_new_recoil_override *>( reinterpret_cast< std::uintptr_t >( this ) + m_new_recoil_override );
	}
};

class c_item_projectile_mod
{
public:

	auto set_projectile_spread( const float spread ) -> void
	{
		driver->write<float>( reinterpret_cast< std::uintptr_t >( this ) + m_projectile_spread, spread );
	}

	auto set_projectile_velocity_spread( const float spread ) -> void
	{
		driver->write<float>( reinterpret_cast< std::uintptr_t >( this ) + m_projectile_velocity_spread, spread );
	}
};

class c_item_definition
{
public:

	auto get_shortname( ) -> const std::wstring
	{
		const auto shortname = driver->read<std::uintptr_t>( reinterpret_cast< std::uintptr_t > ( this ) + m_shortname );

		if ( !shortname )
		{
			return std::wstring( );
		}

		const auto buffer = std::make_unique<wchar_t[]>( 64 );

		driver->read( shortname + 0x14, buffer.get( ), 64 * sizeof( wchar_t ) );

		return std::wstring( buffer.get( ) );
	}
};

class c_primary_magazine
{
public:

	auto get_item_definition( ) -> c_item_definition *
	{
		return driver->read<c_item_definition *>( reinterpret_cast< std::uintptr_t >( this ) + m_item_definition );
	}
};

class c_held
{
public:

	auto set_success_fraction( const float fraction ) -> auto
	{
		driver->write<float>( reinterpret_cast< std::uintptr_t >( this ) + m_success_fraction, fraction );
	}

	auto set_aimcone( const float aimcone ) -> void
	{
		driver->write<float>( reinterpret_cast< std::uintptr_t > ( this ) + m_aimcone, aimcone );
	}

	auto set_hip_aimcone( const float aimcone ) -> void
	{
		driver->write<float>( reinterpret_cast< std::uintptr_t > ( this ) + m_hip_aimcone, aimcone );
	}

	auto set_aimcone_penalty_per_shot( const float penalty ) -> void
	{
		driver->write<float>( reinterpret_cast< std::uintptr_t > ( this ) + m_aimcone_penalty_per_shot, penalty );
	}

	auto set_aimcone_penalty_max( const float penalty ) -> void
	{
		driver->write<float>( reinterpret_cast< std::uintptr_t > ( this ) + m_aimcone_penalty_max, penalty );
	}

	auto set_aimcone_penalty_recover_time( const float penalty ) -> void
	{
		driver->write<float>( reinterpret_cast< std::uintptr_t > ( this ) + m_aimcone_penalty_recover_time, penalty );
	}

	auto set_aimcone_penalty_recover_delay( const float penalty ) -> void
	{
		driver->write<float>( reinterpret_cast< std::uintptr_t > ( this ) + m_aimcone_penalty_recover_delay, penalty );
	}

	auto set_aim_sway( const float sway ) -> void
	{
		driver->write<float>( reinterpret_cast< std::uintptr_t > ( this ) + m_aim_sway, sway );
	}

	auto set_stance_penalty( const float penalty ) -> void
	{
		driver->write<float>( reinterpret_cast< std::uintptr_t > ( this ) + m_stance_penalty, penalty );
	}

	auto set_aimcone_penalty( const float penalty ) -> void
	{
		driver->write<float>( reinterpret_cast< std::uintptr_t > ( this ) + m_aimcone_penalty, penalty );
	}

	auto set_sight_aimcone_scale( const float scale ) -> void
	{
		driver->write<float>( reinterpret_cast< std::uintptr_t > ( this ) + m_sight_aim_cone_scale, scale );
	}

	auto set_sight_aimcone_offset( const float offset ) -> void
	{
		driver->write<float>( reinterpret_cast< std::uintptr_t > ( this ) + m_sight_aim_cone_offset, offset );
	}

	auto set_hip_aimcone_scale( const float scale ) -> void
	{
		driver->write<float>( reinterpret_cast< std::uintptr_t > ( this ) + m_hip_aim_cone_scale, scale );
	}

	auto set_hip_aimcone_offset( const float offset ) -> void
	{
		driver->write<float>( reinterpret_cast< std::uintptr_t > ( this ) + m_hip_aim_cone_offset, offset );
	}

	auto set_aim_sway_speed( const float speed ) -> void
	{
		driver->write<float>( reinterpret_cast< std::uintptr_t > ( this ) + m_aim_sway_speed, speed );
	}

	auto set_did_spark_this_time( const bool sparked ) -> auto
	{
		driver->write<float>( reinterpret_cast< std::uintptr_t >( this ) + m__did_spark_this_frame, sparked );
	}

	auto get_primary_magazine( ) -> c_primary_magazine *
	{
		return driver->read<c_primary_magazine *>( reinterpret_cast< std::uintptr_t > ( this ) + m_primary_magazine );
	}

	auto get_recoil_propreties( ) -> c_recoil_properties *
	{
		return driver->read<c_recoil_properties *>( reinterpret_cast< std::uintptr_t >( this ) + m_recoil );
	}

	auto get_created_projectiles( void ( *each )( std::uintptr_t ) ) -> void
	{
		const auto list = driver->read<std::uintptr_t>( reinterpret_cast< std::uintptr_t >( this ) + m_created_projectiles );

		if ( !list )
		{
			return;
		}

		const auto key = driver->read<std::uintptr_t>( list + 0x10 );

		if ( !key )
		{
			return;
		}

		const auto size = driver->read<int>( list + 0x18 );

		if ( !size )
		{
			return;
		}

		for ( int i = 0; i < size; ++i )
		{
			const auto projectile = driver->read<std::uintptr_t>( key + 0x20 + ( i * 0x8 ) );

			if ( !projectile )
			{
				continue;
			}

			each( projectile );
		}
	}
};

class c_projectile
{
public:

	auto get_item_definition( ) -> c_item_definition *
	{
		return driver->read<c_item_definition *>( reinterpret_cast< std::uintptr_t >( this ) + m_item_definition );
	}

	auto is_weapon( ) -> bool
	{
		const auto item_definition = this->get_item_definition( );

		if ( !item_definition )
		{
			return false;
		}

		const auto name = item_definition->get_shortname( );

		if ( name.empty( ) )
		{
			return false;
		}

		if ( name.find( e( L"rifle" ) ) != std::string::npos )
		{
			return true;
		}

		if ( name.find( e( L"pistol" ) ) != std::string::npos )
		{
			return true;
		}

		if ( name.find( e( L"bow" ) ) != std::string::npos )
		{
			return true;
		}

		if ( name.find( e( L"lmg" ) ) != std::string::npos )
		{
			return true;
		}

		if ( name.find( e( L"shotgun" ) ) != std::string::npos )
		{
			return true;
		}

		if ( name.find( e( L"smg" ) ) != std::string::npos )
		{
			return true;
		}

		return false;
	}

	auto get_uid( ) -> int
	{
		return driver->read<int>( reinterpret_cast< std::uintptr_t > ( this ) + 0x28 );
	}

	auto get_id( ) -> int
	{
		return driver->read_chain<int>( reinterpret_cast< std::uintptr_t > ( this ), { 0x20, 0x18 } );
	}

	auto get_held( ) -> c_held*
	{
		return driver->read<c_held *>( reinterpret_cast< std::uintptr_t >( this ) + m_held_entity );
	}
};

class c_movement
{
public:

	auto set_ground_new_angle( const float angle ) -> void
	{
		driver->write<float>( reinterpret_cast< std::uintptr_t > ( this ) + m_ground_angle_new, angle );
	}

	auto get_ground_new_angle( ) -> const float
	{
		return driver->read<float>( reinterpret_cast< std::uintptr_t > ( this ) + m_ground_angle_new );
	}	
	
	auto set_jump_time( const float time ) -> void
	{
		driver->write<float>( reinterpret_cast< std::uintptr_t > ( this ) + m_jump_time, time );
	}

	auto set_running( const float running ) -> void
	{
		driver->write<float>( reinterpret_cast< std::uintptr_t > ( this ) + m_running, running );
	}

	auto set_grounded( const float grounded ) -> void
	{
		driver->write<float>( reinterpret_cast< std::uintptr_t > ( this ) + m_grounded, grounded );
	}

	auto get_jump_time( ) -> const float
	{
		driver->read<float>( reinterpret_cast< std::uintptr_t > ( this ) + m_jump_time );
	}

	auto set_land_time( const float time ) -> void
	{
		driver->write<float>( reinterpret_cast< std::uintptr_t > ( this ) + m_land_time, time );
	}

	auto get_land_time( ) -> const float
	{
		driver->read<float>( reinterpret_cast< std::uintptr_t > ( this ) + m_land_time );
	}

	auto set_ground_time( const float time ) -> void
	{
		driver->write<float>( reinterpret_cast< std::uintptr_t > ( this ) + m_ground_time, time );
	}

	auto get_ground_time( ) -> const float
	{
		driver->read<float>( reinterpret_cast< std::uintptr_t > ( this ) + m_ground_time );
	}

	auto get_rotation( ) -> const geo::vec4_t
	{
		return driver->read<geo::vec4_t>( reinterpret_cast< std::uintptr_t > ( this ) + m_rotation );
	}

	auto get_position( ) -> const geo::vec3_t
	{
		return driver->read<geo::vec3_t>( reinterpret_cast< std::uintptr_t > ( this ) + m_position );
	}
};

class c_skin_set
{
public:

	auto set_head_material( const std::uintptr_t material ) -> void
	{
		driver->write<std::uintptr_t>( reinterpret_cast< std::uintptr_t >( this ) + m_head_material, material );
	}

	auto set_body_material( const std::uintptr_t material ) -> void
	{
		driver->write<std::uintptr_t>( reinterpret_cast< std::uintptr_t >( this ) + m_body_material, material );
	}

	auto set_eye_material( const std::uintptr_t material ) -> void
	{
		driver->write<std::uintptr_t>( reinterpret_cast< std::uintptr_t >( this ) + m_eye_material, material );
	}

	auto get_head_material( ) -> const std::uintptr_t
	{
		return driver->read<std::uintptr_t>( reinterpret_cast< std::uintptr_t >( this ) + m_head_material );
	}

	auto get_body_material( ) -> const std::uintptr_t
	{
		return driver->read<std::uintptr_t>( reinterpret_cast< std::uintptr_t >( this ) + m_body_material );
	}

	auto get_eye_material( ) -> const std::uintptr_t
	{
		return driver->read<std::uintptr_t>( reinterpret_cast< std::uintptr_t >( this ) + m_eye_material );
	}
};

class c_skin
{
public:

	auto get_skin_set( ) -> c_array<c_skin_set *> *
	{
		return driver->read< c_array<c_skin_set *> *>( reinterpret_cast< std::uintptr_t >( this ) + m_skin_set );
	}
};

class c_model
{
public:

	auto is_local_player( ) -> bool
	{
		return driver->read<bool>( reinterpret_cast< std::uintptr_t > ( this ) + m_is_local_player );
	}

	auto is_visible( ) -> bool
	{
		return driver->read<bool>( reinterpret_cast< std::uintptr_t > ( this ) + m_visible );
	}

	auto get_skin_type( ) -> int
	{
		return driver->read<int>( reinterpret_cast< std::uintptr_t >( this ) + m_skin_type );
	}

	auto get_new_velocity( ) -> geo::vec3_t
	{
		return driver->read<geo::vec3_t>( reinterpret_cast< std::uintptr_t > ( this ) + m_new_velocity );
	}

	auto get_male_skin( ) -> c_skin*
	{
		return driver->read<c_skin*>( reinterpret_cast< std::uintptr_t > ( this ) + m_male_skin );
	}

	auto get_female_skin( ) -> c_skin *
	{
		return driver->read<c_skin *>( reinterpret_cast< std::uintptr_t > ( this ) + m_female_skin );
	}
};

class c_eyes
{
public:

	auto set_body_rotation( const geo::vec4_t quaternion ) -> void
	{
		driver->write<geo::vec4_t>( reinterpret_cast< std::uintptr_t > ( this ) + m_body_rotation, quaternion );
	}

	auto get_body_rotation( ) -> geo::vec4_t
	{
		return driver->read<geo::vec4_t>( reinterpret_cast< std::uintptr_t > ( this ) + m_body_rotation );
	}

	auto set_eye_offset( const geo::vec3_t offset ) -> void
	{
		driver->write < geo::vec3_t> ( reinterpret_cast< std::uintptr_t >( this ) + 0x0, offset );
	}
};

class c_mounted
{
public:

	auto set_can_wield_items( const bool can ) -> void
	{
		driver->write<bool>( reinterpret_cast< std::uintptr_t >( this ) + m_can_wield_items, can );
	}
};

class c_player
{
public:
	auto set_needs_clothes_rebuild( const bool rebuild ) -> void
	{
		driver->write<bool>( reinterpret_cast< std::uintptr_t > ( this ) + m_needs_clothes_rebuild, rebuild );
	}

	auto get_eyes( ) -> c_eyes *
	{
		return driver->read<c_eyes *>( reinterpret_cast< std::uintptr_t > ( this ) + m_eyes );
	}

	auto get_movement( ) -> c_movement *
	{
		return driver->read<c_movement *>( reinterpret_cast< std::uintptr_t > ( this ) + m_base_movement );
	}

	auto get_health( ) -> float
	{
		return driver->read<float>( reinterpret_cast< std::uintptr_t > ( this ) + m__health );
	}

	auto get_max_health( ) -> float
	{
		return driver->read<float>( reinterpret_cast< std::uintptr_t > ( this ) + m__max_health );
	}

	auto get_mounted( ) -> c_mounted *
	{
		return driver->read<c_mounted *>( reinterpret_cast< std::uintptr_t > ( this ) + m_mounted );
	}

	auto get_model( ) -> c_model *
	{
		return driver->read<c_model *>( reinterpret_cast< std::uintptr_t > ( this ) + m_model );
	}

	auto get_steam_id( ) -> std::uintptr_t
	{
		return driver->read<std::uintptr_t>( reinterpret_cast< std::uintptr_t > ( this ) + m_user_id );
	}

	auto is_dead( ) -> bool
	{
		return driver->read<bool>( reinterpret_cast< std::uintptr_t > ( this ) + m_life_state );
	}

	auto has_flag( std::uint32_t flag ) -> bool
	{
		return ( this->get_player_flags( ) & flag );
	}

	auto is_valid( bool local_player = false ) -> bool
	{
		if ( !this )
		{
			return false;
		}

		const auto health = this->get_health( );

		if ( health > 150.f || health < 0.8f )
		{
			return false;
		}

		if ( local_player )
		{
			return !is_dead( );
		}

		return ( !this->has_flag( 16 ) && !is_dead( ) );
	}

	auto set_clothing_blocks_aiming( bool block )
	{
		driver->write<bool>( reinterpret_cast< std::uintptr_t >( this ) + m_clothing_blocks_aiming, block );
	}

	auto set_clothing_move_speed_reduction( float reduction )
	{
		driver->write<float>( reinterpret_cast< std::uintptr_t >( this ) + m_clothing_move_speed_reduction, reduction );
	}

	auto set_clothing_water_speed_bonus( float bonus )
	{
		driver->write<float>( reinterpret_cast< std::uintptr_t >( this ) + m_clothing_water_speed_bonus, bonus );
	}

	auto set_clothing_accuracy_bonus( float bonus )
	{
		driver->write<float>( reinterpret_cast< std::uintptr_t >( this ) + m_clothing_accuracy_bonus, bonus );
	}

	auto get_current_team( ) -> int
	{
		return driver->read<int>( reinterpret_cast< std::uintptr_t > ( this ) + m_current_team );
	}

	auto get_player_flags( ) -> std::uint32_t
	{
		return driver->read<std::uint32_t>( reinterpret_cast< std::uintptr_t > ( this ) + m_player_flags );
	}

	auto set_player_flags( const std::uint32_t flags ) -> void
	{
		driver->write<std::uint32_t>( reinterpret_cast< std::uintptr_t > ( this ) + m_player_flags, flags );
	}

	auto get_last_headshot_sound_time( ) -> float
	{
		return driver->read<float>( reinterpret_cast< std::uintptr_t > ( this ) + m_last_headshot_sound_time );
	}

	auto get_projectile( ) -> c_projectile *
	{
		const auto active_uid = driver->read<int>( reinterpret_cast< std::uintptr_t > ( this ) + m_cl_active_item );
		
		if ( !active_uid )
		{
			return nullptr;
		}

		for ( int i = 0; i < 6; i++ )
		{
			const auto projectile = driver->read_chain<c_projectile *>( reinterpret_cast< std::uintptr_t > ( this ), { m_inventory, 0x28, 0x38, 0x10, 0x20ull + ( i * 0x8 ) } );

			if ( !projectile )
			{
				continue;
			}

			if ( active_uid == projectile->get_uid() )
			{
				return projectile;
			}
		}

		return nullptr;
	}

	auto get_name( ) -> std::wstring
	{
		const auto shortname = driver->read<std::uintptr_t>( reinterpret_cast< std::uintptr_t > ( this ) + m__display_name );

		if ( !shortname )
		{
			return std::wstring( );
		}

		const auto buffer = std::make_unique<wchar_t[]>( 64 );

		driver->read( shortname + 0x14, buffer.get( ), 64 * sizeof( wchar_t ) );

		return std::wstring( buffer.get( ) );
	}

	template <typename t>
	auto get_transform( const std::uint32_t i ) -> t
	{
		return driver->read_chain<t>( reinterpret_cast< std::uintptr_t > ( this ), { 0x130, 0x48, 0x20ull + ( i * 0x8 ), 0x10 } );
	}
};