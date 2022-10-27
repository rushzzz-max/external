namespace local
{
	auto fast_loop( ) -> void
	{
		while ( true )
		{
			if ( !globals::game.shown || !cache::local_player || !cache::camera || !cache::local_player->is_valid( true ) )
			{
				Sleep( 100 );
				continue;
			}

			features::quick_process( cache::local_player );
		}
	}

	auto self_loop( ) -> void
	{
		while ( true )
		{
			if ( !globals::game.shown )
			{
				Sleep( 100 );
				continue;
			}

			menu::add_title( e( "VISUALS" ) );
			menu::add_toggle( e( "players" ), &settings->visuals.players );
			menu::add_toggle( e( "barrels" ), &settings->visuals.barrels );
			menu::add_toggle( e( "collectables" ), &settings->visuals.collectables );
			menu::add_toggle( e( "crates" ), &settings->visuals.crates );
			menu::add_toggle( e( "loots" ), &settings->visuals.loots );
			menu::add_toggle( e( "ores" ), &settings->visuals.ores );
			menu::add_toggle( e( "vehicules" ), &settings->visuals.vehicules );
			menu::add_toggle( e( "traps" ), &settings->visuals.traps );
			menu::add_slider( e( "max distance" ), &settings->visuals.max_distance, 0, 300 );

			menu::add_title( e( "AIM" ) );
			menu::add_toggle( e( "silent aim" ), &settings->aim.silent_aim );
			menu::add_toggle( e( "blatant" ), &settings->aim.blatant );
			menu::add_slider( e( "fov" ), &settings->aim.fov, 0, 1000, 10 );
			menu::add_toggle( e( "team check" ), &settings->aim.team_check );
			menu::add_slider( e( "max distance" ), &settings->aim.max_distance, 0, 300 );

			menu::add_title( e( "WEAPON" ) );
			menu::add_toggle( e( "no spread" ), &settings->weapon.no_spread );
			menu::add_toggle( e( "no recoil" ), &settings->weapon.no_recoil );
			menu::add_toggle( e( "super eoka" ), &settings->weapon.super_eoka );
			menu::add_toggle( e( "fat bullet" ), &settings->weapon.fat_bullet );
			menu::add_toggle( e( "no sway" ), &settings->weapon.no_sway );

			menu::add_title( e( "MISC" ) );
			menu::add_toggle( e( "chams" ), &settings->misc.chams );
			menu::add_toggle( e( "night mode" ), &settings->misc.night_mode );
			menu::add_toggle( e( "fake admin" ), &settings->misc.fake_admin );
			menu::add_toggle( e( "shoot in copter" ), &settings->misc.shoot_in_copter );

			menu::add_title( e( "MOVEMENT" ) );
			menu::add_toggle( e( "speedhack" ), &settings->movement.speedhack );
			menu::add_toggle( e( "omnisprint" ), &settings->movement.onmisprint );
			menu::add_toggle( e( "infinite jump" ), &settings->movement.infinite_jump );
			menu::add_toggle( e( "spiderman" ), &settings->movement.spiderman );

			menu::reset( );

			if ( !cache::local_player || !cache::camera || !cache::local_player->is_valid( true ) )
			{
				continue;
			}

			features::process( cache::local_player );

			if ( !aim::target( ) )
			{
				cache::targeted = nullptr;
			}
		}
	}
}