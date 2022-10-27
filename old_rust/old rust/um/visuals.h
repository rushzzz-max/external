namespace visuals
{
	void draw_loop( )
	{
		SetThreadPriority( GetCurrentThread( ), THREAD_PRIORITY_HIGHEST );

		while ( true )
		{
			if ( !cache::camera || !globals::game.shown )
			{
				Sleep( 100 );
				continue;
			}

			const auto matrix = cache::camera->get_view_matrix( );

			for ( auto &player : cache::players )
			{
				auto head_3d = ( std::get<0>( player )->get_transform< c_transform *>( 47 )->position( ) + geo::vec3_t( 0, 0.23, 0 ) );
				
				if ( head_3d.empty( ) )
				{
					continue;
				}

				auto head_2d = cache::camera->world_to_screen( matrix, head_3d );

				if ( ( int )head_2d.x == 0 || ( int )head_2d.y == 0 )
				{
					continue;
				}

				auto feet_3d = geo::vec3_t( std::get<0>( player )->get_transform< c_transform *>( 3 )->position( ) + std::get<0>( player )->get_transform< c_transform *>( 15 )->position() ) / 2.f;
				feet_3d.y -= 0.2f;

				if ( feet_3d.empty( ) )
				{
					continue;
				}

				const auto feet_2d = cache::camera->world_to_screen( matrix, feet_3d );

				if ( ( int )feet_2d.x == 0 || ( int )feet_2d.y == 0 )
				{
					continue;
				}

				const auto distance = head_3d.distance( cache::local_player->get_transform< c_transform *>( 47 )->position( ) );

				const auto h = ( head_2d.y - feet_2d.y );
				const auto w = h / 1.8f;
				const auto x = feet_2d.x - ( w / 2.f );
				const auto y = feet_2d.y;

				if ( std::get<2>( player ) )
				{
					driver->draw_rect( x, y, w, h, 2, 7, 127, 7 );
					driver->draw_rect( x, y, w, h, 2, 15, 255, 15 );
				}
				else
				{
					if ( std::get<1>( player )->is_visible( ) )
					{
						if ( std::get<0>( player ) == cache::targeted )
						{
							driver->draw_rect( x, y, w, h, 2, 127, 0, 0 );
							driver->draw_rect( x, y, w, h, 1, 255, 0, 0 );
						}
						else
						{
							driver->draw_rect( x, y, w, h, 2, 127, 93, 0 );
							driver->draw_rect( x, y, w, h, 1, 255, 187, 0 );
						}
					}
					else
					{
						driver->draw_rect( x, y, w, h, 2, 127, 127, 127 );
						driver->draw_rect( x, y, w, h, 1, 255, 255, 255 );
					}
				}

				wchar_t bdistance[15] = { };
				swprintf( bdistance, e( L"%iM" ), ( int )distance );
				driver->draw_text( x + ( w / 2.f ), y + h - 33, bdistance, 255, 255, 255 );

				wchar_t bhealth[15] = { };
				swprintf( bhealth, e( L"%s [%iHP]" ), std::get<3>( player ).c_str(), ( int )( std::get<0>( player )->get_health( ) + 0.4f ) );
				driver->draw_text( x + ( w / 2.f ), y + h - 17, bhealth, 255, 255, 255 );
			}

			for ( auto &other : cache::others )
			{
				auto position_2d = cache::camera->world_to_screen( matrix, std::get<0>( other ) );

				if ( ( int )position_2d.x == 0 || ( int )position_2d.y == 0 )
				{
					continue;
				}
				
				driver->draw_text( position_2d.x, position_2d.y, std::get<1>( other ), 255, 174, 0 );
			}
		}
	}
}