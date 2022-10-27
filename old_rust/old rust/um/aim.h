namespace aim
{
	std::map<std::wstring, float> bullets = 
	{
		{ e ( L"ammo.rifle" ), 1.0f },
		{ e ( L"ammo.rifle.hv" ), 1.2f },
		{ e ( L"ammo.rifle.explosive" ), 0.49f },
		{ e ( L"ammo.rifle.incendiary" ), 0.55f },

		{ e ( L"ammo.pistol" ), 1.0f },
		{ e ( L"ammo.pistol.hv" ), 1.33333f },
		{ e ( L"ammo.pistol.fire" ), 0.75f },

		{ e ( L"arrow.wooden" ), 1.0f },
		{ e ( L"arrow.hv" ), 1.6f },
		{ e ( L"arrow.fire" ), 0.8f },
		{ e ( L"arrow.bone" ), 0.9f },

		{ e ( L"ammo.handmade.shell" ), 1.0f },
		{ e ( L"ammo.shotgun.slug" ), 2.25f },
		{ e ( L"ammo.shotgun.fire" ), 1.0f },
		{ e ( L"ammo.shotgun" ), 2.25f },

		{ e ( L"ammo.nailgun.nails" ), 1.0f }
	};

	auto get_projectile_speed( c_projectile* projectile ) -> const float
	{
		const auto item_definition = projectile->get_item_definition();

		if ( !item_definition )
		{
			return 300.f;
		}

		const auto shortname = item_definition->get_shortname( );

		if ( shortname.empty( ) )
		{
			return 300.f;
		}

		const auto held = projectile->get_held( );

		if ( !held )
		{
			return 300.f;
		}

		const auto primary_magazine = held->get_primary_magazine( );

		if ( !primary_magazine )
		{
			return 300.f;
		}

		const auto magazine_item_definition = primary_magazine->get_item_definition( );

		if ( !magazine_item_definition )
		{
			return 300.f;
		}

		const auto magazine_shortname = magazine_item_definition->get_shortname( );

		if ( magazine_shortname.empty( ) )
		{
			return 300.f;
		}

		if      ( shortname == e( L"rifle.ak" ) )                return 360.0f * bullets[magazine_shortname];
		if		( shortname == e( L"hmlmg" ) )					 return 503 * bullets[magazine_shortname];
		else if ( shortname == e( L"rifle.lr300" ) )             return 340.2f * bullets[magazine_shortname];
		else if ( shortname == e( L"rifle.bolt" ) )              return 542.f * bullets[magazine_shortname];
		else if ( shortname == e( L"rifle.l96" ) )               return 1125.0f * bullets[magazine_shortname];
		else if ( shortname == e( L"rifle.m39" ) )               return 674.f * bullets[magazine_shortname];
		else if ( shortname == e( L"rifle.semiauto" ) )			 return 360.0f * bullets[magazine_shortname];
		else if ( shortname == e( L"lmg.m249" ) )                return 560.0f * bullets[magazine_shortname];
		else if ( shortname == e( L"smg.thompson" ) )            return 272.0f * bullets[magazine_shortname];
		else if ( shortname == e( L"smg.2" ) )					 return 240.0f * bullets[magazine_shortname];
		else if ( shortname == e( L"smg.mp5" ) )                 return 222.8f * bullets[magazine_shortname];
		else if ( shortname == e( L"pistol.python" ) )           return 300.0f * bullets[magazine_shortname];
		else if ( shortname == e( L"pistol.semiauto" ) )         return 300.0f * bullets[magazine_shortname];
		else if ( shortname == e( L"pistol.revolver" ) )         return 270.0f * bullets[magazine_shortname];
		else if ( shortname == e( L"pistol.m92" ) )              return 300.0f * bullets[magazine_shortname];
		else if ( shortname == e( L"pistol.eoka" ) )             return 90.0f * bullets[magazine_shortname];
		else if ( shortname == e( L"pistol.nailgun" ) )          return 56.55f;
		else if ( shortname == e( L"crossbow" ) )                return 88.0f * bullets[magazine_shortname];
		else if ( shortname == e( L"bow.compound" ) )            return 100.0f * bullets[magazine_shortname];
		else if ( shortname == e( L"bow.hunting" ) )             return 57.0f * bullets[magazine_shortname];
		else if ( shortname == e( L"shotgun.pump" ) )            return 100.0f * bullets[magazine_shortname];
		else if ( shortname == e( L"shotgun.spas12" ) )          return 100.0f * bullets[magazine_shortname];
		else if ( shortname == e( L"shotgun.waterpipe" ) )       return 100.0f * bullets[magazine_shortname];
		else if ( shortname == e( L"shotgun.doublebarrel" ) )    return 100.0f * bullets[magazine_shortname];

		return 300.0f;
	}

	auto predict_position ( float distance, float bullet_speed, geo::vec3_t velocity, geo::vec3_t position ) -> geo::vec3_t
	{
		if ( distance > 0.001f )
		{
			const auto time = distance / bullet_speed;
			
			velocity.x *= 0.55f * time;
			velocity.y *= 0.55f * time;
			velocity.z *= 0.55f * time;

			position.x += velocity.x;
			position.y += velocity.y;
			position.z += velocity.z;

			position.y += ( 4.905f * time * time ) + 0.08;
		}

		return position;
	};

	auto target( ) -> bool
	{
		if ( !settings->aim.silent_aim )
		{
			return false;
		}

		driver->draw_rect( globals::game.w / 2.f - settings->aim.fov / 2.f, globals::game.h / 2.f - settings->aim.fov / 2.f, settings->aim.fov, settings->aim.fov, 1, 255, 255, 255 );

		if ( !GetAsyncKeyState( VK_RBUTTON ) )
		{
			return false;
		}

		const auto projectile = cache::local_player->get_projectile( );

		if ( !projectile )
		{
			return false;
		}

		if ( !projectile->is_weapon( ) )
		{
			return false;
		}

		const auto matrix = cache::camera->get_view_matrix( );

		std::tuple<c_player *, float, float> target { nullptr, FLT_MAX, FLT_MAX };

		for ( auto& entity : cache::players )
		{
			const auto team = std::get<2>( entity );

			if ( team && settings->aim.team_check )
			{
				continue;
			}

			const auto player = std::get<0>( entity );

			if ( !player->is_valid( ) || player == cache::local_player )
			{
				continue;
			}

			auto position_3d = player->get_transform<c_transform *>( 47 )->position( );

			if ( position_3d.empty( ) )
			{
				continue;
			}

			const auto distance_3d = position_3d.distance( cache::local_player->get_transform<c_transform *>( 47 )->position( ) );

			if ( distance_3d > settings->aim.max_distance )
			{
				continue;
			}

			auto position_2d = cache::camera->world_to_screen( matrix, position_3d );

			if ( ( int )position_2d.z == 0 )
			{
				continue;
			}

			if ( !geo::in_square( { position_2d.x, position_2d.y },
				{ globals::game.w / 2.f - settings->aim.fov / 2.f, globals::game.h / 2.f - settings->aim.fov / 2.f }, settings->aim.fov ) )
			{
				continue;
			}

			const auto distance_2d = geo::vec2_t( position_2d.x, position_2d.y ).distance( { globals::game.w / 2.f, globals::game.h / 2.f } );

			if ( distance_2d >= std::get<1>( target )  )
			{
				continue;
			}

			std::get<2>( target ) = distance_3d;
			std::get<1>( target ) = distance_2d;
			std::get<0>( target ) = player;
		}

		if ( std::get<1>( target ) == FLT_MAX )
		{
			return false;
		}

		const auto distance_3d = std::get<2>( target );

		if ( distance_3d == FLT_MAX )
		{
			return false;
		}

		const auto player = std::get<0>( target );

		if ( !player )
		{
			return false;
		}

		const auto model = player->get_model( );

		auto position_3d = aim::predict_position( 
			distance_3d,
			get_projectile_speed( projectile ),
			model->get_new_velocity(),
			player->get_transform<c_transform *>( 47 )->position() );

		if ( position_3d.empty( ) )
		{
			return false;
		}

		auto position_2d = cache::camera->world_to_screen( matrix, position_3d );

		if ( ( int )position_2d.z == 0 )
		{
			return false;
		}

		driver->draw_text( position_2d.x, position_2d.y, L"+", 255, 0, 0 );

		auto local_3d = cache::local_player->get_transform<c_transform *>( 47 )->position( );

		if ( local_3d.empty( ) )
		{
			return false;
		}

		auto angles = geo::calculate_angle( local_3d, position_3d );

		if ( std::isnan( angles.x ) || std::isnan( angles.y ) )
		{
			return false;
		}

		geo::clamp( angles.x, angles.y );

		const auto eyes = cache::local_player->get_eyes( );

		if ( !eyes )
		{
			return false;
		}

		eyes->set_body_rotation( geo::to_quaternion( angles ) );

		cache::targeted = player;

		return true;
	}
}