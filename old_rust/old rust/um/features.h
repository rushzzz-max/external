namespace features
{
	auto no_recoil( c_recoil_properties *recoil_propreties, c_new_recoil_override *new_recoil_override ) -> void
	{
		recoil_propreties->set_recoil_pitch_max( 0.f );
		recoil_propreties->set_recoil_pitch_min( 0.f );
		recoil_propreties->set_recoil_yaw_max( 0.f );
		recoil_propreties->set_recoil_yaw_min( 0.f );

		new_recoil_override->set_recoil_pitch_max( 0.f );
		new_recoil_override->set_recoil_pitch_min( 0.f );
		new_recoil_override->set_recoil_yaw_max( 0.f );
		new_recoil_override->set_recoil_yaw_min( 0.f );
	}

	auto infinite_jump( c_movement *movement ) -> void
	{
		if ( !GetAsyncKeyState( VK_SPACE ) )
		{
			return;
		}

		movement->set_jump_time( 0.51f );
		movement->set_land_time( 0.3f );
		movement->set_ground_time( 99999.f );
	}

	auto spiderman( c_movement *movement ) -> void
	{
		movement->set_ground_new_angle( 0.f );
	}

	auto fat_bullet( c_held *held )
	{
		auto set_thickness = []( std::uintptr_t each ) -> void
		{
			driver->write<float>( each + m_thickness, 1.f );
		};

		held->get_created_projectiles( set_thickness );
	}

	auto admin_flags( c_player *player ) -> void
	{
		auto flags = player->get_player_flags( );

		flags |= 4;

		player->set_player_flags( flags );

		const auto convar_client = cache::game_assembly->get_convar_client( );

		if ( convar_client )
		{
			convar_client->set_camlerp( 1.f );
			convar_client->set_camlookspeed( 1.f );
			convar_client->set_camspeed( 1.f );
		}
	}

	auto chams( c_player *player, c_tod_sky* tod_sky, c_skinnable* skinnable )
	{
		const auto tod_components = tod_sky->get_tod_components( );

		if ( !tod_components )
		{
			return;
		}

		const auto tod_scattering = tod_components->get_scattering( );

		if ( !tod_scattering )
		{
			return;
		}

		const auto material = tod_scattering->get_material( );

		if ( !material )
		{
			return;
		}

		const auto skinnables = skinnable->get_all( );

		for ( std::uint32_t i = 0; i < skinnables->size( ); i++ )
		{
			const auto skin = skinnables->get( i );

			if ( !skin )
			{
				continue;
			}

			const auto category = skin->get_category( );

			if ( category == category::footwear || category == category::mask || category == category::gloves || category == category::hat || 
				category == category::jacket || category == category::pants || category == category::shirt )
			{
				const auto groups = skin->get_groups( );

				for ( std::uint32_t i = 0; i < groups->size( ); i++ )
				{
					const auto group = groups->get( i );

					if ( !group )
					{
						continue;
					}

					group->set_material( material );
				}
			}
		}

		const auto model = player->get_model( );

		if ( !model )
		{
			return;
		}

		const auto skin = ( model->get_skin_type( ) == 1 ? model->get_female_skin( ) : model->get_male_skin( ) );

		if ( !skin )
		{
			return;
		}

		const auto skin_set = skin->get_skin_set( );

		if ( !skin_set )
		{
			return;
		}

		for ( std::uint8_t i = 0; i < skin_set->size( ); i++ )
		{
			const auto set = skin_set->get( i );

			if ( !set )
			{
				continue;
			}

			set->set_eye_material( material );
			set->set_head_material( material );
			set->set_body_material( material );
		}
	}

	auto shoot_in_copter( c_mounted *mounted ) -> void
	{
		mounted->set_can_wield_items( true );
	}

	auto night_mode( c_tod_sky *tod_sky ) -> void
	{
		const auto tod_ambient = tod_sky->get_tod_aimbient( );

		if ( tod_ambient )
		{
			tod_ambient->set_update_interval( 1.f );
		}

		const auto tod_night = tod_sky->get_tod_night( );

		if ( tod_night )
		{
			tod_night->set_ambient_multiplier( 3.2f );
		}

		const auto tod_cycle = tod_sky->get_tod_cycle( );

		if ( tod_cycle )
		{
			tod_cycle->set_hour( 0.f );
		}
	}

	auto omnisprint( c_movement *movement )-> void
	{
		movement->set_running( 1.f );
	}

	auto quick_process( c_player *player ) -> void
	{
		const auto movement = player->get_movement( );

		if ( settings->movement.onmisprint )
		{
			features::omnisprint( movement );
		}

		if ( settings->misc.night_mode )
		{
			features::night_mode( cache::tod_sky );
		}
	}

	auto speedhack( c_player *player, const bool status ) -> void
	{
		player->set_clothing_move_speed_reduction( ( status ? -6.f : 0.f ) );
	}

	auto no_sway( c_held *held ) -> void
	{
		held->set_aim_sway( 0.f );
		held->set_aim_sway_speed( 0.f );
	}

	auto no_spread( c_held *held ) -> void
	{
		held->set_aimcone( 0.f );
		held->set_hip_aimcone( 0.f );
		held->set_aimcone_penalty_per_shot( 0.f );
		held->set_aimcone_penalty_max( 0.f );
		held->set_aimcone_penalty_recover_time( 0.f );
		held->set_aimcone_penalty_recover_delay( 0.f );
		held->set_stance_penalty( 0.f );
		held->set_aimcone_penalty( 0.f );
		held->set_sight_aimcone_scale( 0.f );
		held->set_sight_aimcone_offset( 0.f );
		held->set_hip_aimcone_scale( 0.f );
		held->set_hip_aimcone_offset( 0.f );
	}

	auto super_eoka( c_projectile* projectile, c_held *held ) -> void
	{
		if ( projectile->get_item_definition()->get_shortname().find( e( L"eoka" ) ) == std::string::npos )
		{
			return;
		}

		auto set_eoka = []( const std::uintptr_t rcx ) -> void
		{
			const auto item_projectile_mod = driver->read<c_item_projectile_mod *>( rcx + m_mod );

			if ( !item_projectile_mod )
			{
				return;
			}

			item_projectile_mod->set_projectile_spread( 0.f );
			item_projectile_mod->set_projectile_velocity_spread( 0.f );
		};

		held->get_created_projectiles( set_eoka );

		held->set_success_fraction( 1.f );
	}

	auto process( c_player *player ) -> void
	{
		if ( settings->misc.fake_admin )
		{
			features::admin_flags( player );
		}

		const auto movement = player->get_movement( );

		if ( movement )
		{
			if ( settings->movement.onmisprint )
			{
				features::omnisprint( movement );
			}

			if ( settings->movement.infinite_jump )
			{
				features::infinite_jump( movement );
			}

			if ( settings->movement.spiderman )
			{
				features::spiderman( movement );
			}
		}

		features::speedhack( player, settings->movement.speedhack );

		if ( settings->misc.shoot_in_copter )
		{
			const auto mounted = player->get_mounted( );

			if ( mounted )
			{
				features::shoot_in_copter( mounted );
			}
		}

		const auto projectile = player->get_projectile( );

		if ( projectile && projectile->is_weapon( ) )
		{
			const auto held = projectile->get_held( );

			if ( held )
			{
				if ( settings->weapon.fat_bullet )
				{
					features::fat_bullet( held );
				}

				const auto recoil_propreties = held->get_recoil_propreties( );

				if ( recoil_propreties )
				{
					const auto new_recoil_override = recoil_propreties->get_new_recoil_override( );

					if ( new_recoil_override )
					{
						if ( settings->weapon.no_recoil )
						{
							features::no_recoil( recoil_propreties, new_recoil_override );
						}
					}
				}

				if ( settings->weapon.no_sway )
				{
					features::no_sway( held );
				}

				if ( settings->weapon.no_spread )
				{
					features::no_spread( held );
				}

				if ( settings->weapon.super_eoka )
				{
					features::super_eoka( projectile, held );
				}
			}
		}
	}
}