namespace core
{
	void entity_loop( void ( *on_update )( ) )
	{
		SetThreadPriority( GetCurrentThread( ), THREAD_PRIORITY_LOWEST );

		cache::game_assembly = driver->get_module_base< c_game_assembly* > ( e( L"GameAssembly.dll" ) );

		cache::unity = driver->get_module_base< c_unity * >( e( L"UnityPlayer.dll" ) );

		while ( true )
		{
			on_update( );

			cache::tod_sky = cache::game_assembly->get_tod_sky( );

			cache::skinnable = cache::game_assembly->get_skinnable( );

			const auto base_entity = cache::game_assembly->get_base_entity( );
			
			if ( !base_entity )
			{
				continue;
			}

			const auto client_entities = base_entity->get_client_entities( );

			if ( !client_entities )
			{
				continue;
			}

			std::vector < std::tuple < c_player *, c_model *, bool, std::wstring > > players { };
			std::vector<std::tuple<geo::vec3_t, const wchar_t*>> others { };
			
			for ( std::uint32_t i = 0; i < client_entities->size(); i++ )
			{
				const auto object = driver->read<std::uintptr_t>( client_entities->get( i ) + 0x10 );

				if ( !object )
				{
					continue;
				}

				const auto object_class = driver->read<std::uintptr_t>( object + 0x30 );

				if ( !object_class )
				{
					continue;
				}

				const auto tag = driver->read<WORD>( object_class + 0x54 );

				const auto name = driver->read_chain<unity_str>( object_class, { 0x60, 0x0 } ).buffer;

				if ( settings->visuals.players && tag == 6 )
				{
					const auto player = driver->read<c_player *>( object + 0x28 );

					if ( !player )
					{
						continue;
					}

					if ( !player->is_valid( ) )
					{
						continue;
					}

					if ( i == 0 )
					{
						cache::camera = cache::unity->get_camera( );
						cache::local_player = player;
					}
					else
					{
						const auto distance = player->get_transform<c_transform *>( 47 )->position( ).distance( cache::local_player->get_transform< c_transform *>( 47 )->position( ) );

						if ( distance > settings->visuals.max_distance )
						{
							continue;
						}

						if ( settings->misc.chams )
						{
							features::chams( player, cache::tod_sky, cache::skinnable );
						}

						player->set_needs_clothes_rebuild( true );

						const auto local_team = cache::local_player->get_current_team( );
						const auto player_team = player->get_current_team( );

						const bool team = ( local_team == player_team && local_team && player_team );

						players.push_back( { player, player->get_model( ), team, player->get_name( ) } );
					}
					
					continue;
				}

				std::tuple<geo::vec3_t, const wchar_t *> element { { 0.f, 0.f, 0.f }, L"" };
				if ( settings->visuals.loots )
				{
					if ( strstr( name, e( "small_stash_deployed.prefab" ) ) )
					{
						element = std::tuple<geo::vec3_t, const wchar_t *>( { driver->read_chain<geo::vec3_t>( object_class, { 0x30, 0x8, 0x38, 0x90 } ), e( L"stash") } );
						goto end;
					}
					else if ( strstr( name, e( "codelockedhackablecrate.prefab" ) ) )
					{
						element = std::tuple<geo::vec3_t, const wchar_t *>( { driver->read_chain<geo::vec3_t>( object_class, { 0x30, 0x8, 0x38, 0x90 } ), e( L"locked create") } );
						goto end;
					}
					else if ( strstr( name, e( "supply_drop.prefab" ) ) )
					{
						element = std::tuple<geo::vec3_t, const wchar_t *>( { driver->read_chain<geo::vec3_t>( object_class, { 0x30, 0x8, 0x38, 0x90 } ), e( L"air drop") } );
						goto end;
					}
					else if ( strstr( name, e( "player_corpse.prefab" ) ) )
					{
						element = std::tuple<geo::vec3_t, const wchar_t *>( { driver->read_chain<geo::vec3_t>( object_class, { 0x30, 0x8, 0x38, 0x90 } ), e( L"corpse") } );
						goto end;
					}
					else if ( strstr( name, e( "item_drop_backpack.prefab" ) ) )
					{
						element = std::tuple<geo::vec3_t, const wchar_t *>( { driver->read_chain<geo::vec3_t>( object_class, { 0x30, 0x8, 0x38, 0x90 } ), e( L"backpack") } );
						goto end;
					}
				}
				
				if ( settings->visuals.collectables )
				{
					if ( strstr( name, e( "hemp-collectable.prefab" ) ) )
					{
						element = std::tuple<geo::vec3_t, const wchar_t *>( { driver->read_chain<geo::vec3_t>( object_class, { 0x30, 0x8, 0x38, 0x90 } ), e( L"hemp") } );
						goto end;
					}
					else if ( strstr( name, e( "stone-collectable.prefab" ) ) )
					{
						element = std::tuple<geo::vec3_t, const wchar_t *>( { driver->read_chain<geo::vec3_t>( object_class, { 0x30, 0x8, 0x38, 0x90 } ), e( L"stone") } );
						goto end;
					}
					else if ( strstr( name, e( "metal-collectable.prefab" ) ) )
					{
						element = std::tuple<geo::vec3_t, const wchar_t *>( { driver->read_chain<geo::vec3_t>( object_class, { 0x30, 0x8, 0x38, 0x90 } ), e( L"metal") } );
						goto end;
					}
					else if ( strstr( name, e( "sulfur-collectable.prefab" ) ) )
					{
						element = std::tuple<geo::vec3_t, const wchar_t *>( { driver->read_chain<geo::vec3_t>( object_class, { 0x30, 0x8, 0x38, 0x90 } ), e( L"sulfur") } );
						goto end;
					}
					else if ( strstr( name, e( "wood-collectable.prefab" ) ) )
					{
						element = std::tuple<geo::vec3_t, const wchar_t *>( { driver->read_chain<geo::vec3_t>( object_class, { 0x30, 0x8, 0x38, 0x90 } ), e( L"wood") } );
						goto end;
					}
				}

				if ( settings->visuals.vehicules )
				{
					if ( strstr( name, e( "minicopter.entity.prefab" ) ) )
					{
						element = std::tuple<geo::vec3_t, const wchar_t *>( { driver->read_chain<geo::vec3_t>( object_class, { 0x30, 0x8, 0x38, 0x90 } ), e( L"minicopter") } );
						goto end;
					}
					else if ( strstr( name, e( "patrolhelicopter.prefab" ) ) )
					{
						element = std::tuple<geo::vec3_t, const wchar_t *>( { driver->read_chain<geo::vec3_t>( object_class, { 0x30, 0x8, 0x38, 0x90 } ), e( L"patrol heli") } );
						goto end;
					}
					else if ( strstr( name, e( "scraptransporthelicopter.prefab" ) )  )
					{
						element = std::tuple<geo::vec3_t, const wchar_t *>( { driver->read_chain<geo::vec3_t>( object_class, { 0x30, 0x8, 0x38, 0x90 } ), e( L"scrap heli") } );
						goto end;
					}
					else if ( strstr( name, e( "rowboat.prefab" ) ) )
					{
						element = std::tuple<geo::vec3_t, const wchar_t *>( { driver->read_chain<geo::vec3_t>( object_class, { 0x30, 0x8, 0x38, 0x90 } ), e( L"boat") } );
						goto end;
					}
					else if ( strstr( name, e( "rhib.prefab" ) ) )
					{
						element = std::tuple<geo::vec3_t, const wchar_t *>( { driver->read_chain<geo::vec3_t>( object_class, { 0x30, 0x8, 0x38, 0x90 } ), e( L"rhib") } );
						goto end;
					}
					else if ( strstr( name, e( "bradleyapc.prefab" ) ) )
					{
						element = std::tuple<geo::vec3_t, const wchar_t *>( { driver->read_chain<geo::vec3_t>( object_class, { 0x30, 0x8, 0x38, 0x90 } ), e( L"bradley") } );
						goto end;
					}
					else if ( strstr( name, e( "kayak.prefab" ) ) )
					{
						element = std::tuple<geo::vec3_t, const wchar_t *>( { driver->read_chain<geo::vec3_t>( object_class, { 0x30, 0x8, 0x38, 0x90 } ), e( L"kayak") } );
						goto end;
					}
					else if ( strstr( name, e( "horse.prefab" ) ) )
					{
						element = std::tuple<geo::vec3_t, const wchar_t *>( { driver->read_chain<geo::vec3_t>( object_class, { 0x30, 0x8, 0x38, 0x90 } ), e( L"horse" ) } );
						goto end;
					}
				}

				if ( settings->visuals.ores )
				{
					if ( strstr( name, e( "stone-ore.prefab" ) ) )
					{
						element = std::tuple<geo::vec3_t, const wchar_t *>( { driver->read_chain<geo::vec3_t>( object_class, { 0x30, 0x8, 0x38, 0x90 } ), e( L"stone ore") } );
						goto end;
					}
					else if ( strstr( name, e( "metal-ore.prefab" ) ) )
					{
						element = std::tuple<geo::vec3_t, const wchar_t *>( { driver->read_chain<geo::vec3_t>( object_class, { 0x30, 0x8, 0x38, 0x90 } ), e( L"metal ore") } );
						goto end;
					}
					else if ( strstr( name, e( "sulfur-ore.prefab" ) ) )
					{
						element = std::tuple<geo::vec3_t, const wchar_t *>( { driver->read_chain<geo::vec3_t>( object_class, { 0x30, 0x8, 0x38, 0x90 } ), e( L"sulfur ore") } );
						goto end;
					}
				}

				if ( settings->visuals.crates )
				{
					if ( strstr( name, e( "crate_elite.prefab" ) ) )
					{
						element = std::tuple<geo::vec3_t, const wchar_t *>( { driver->read_chain<geo::vec3_t>( object_class, { 0x30, 0x8, 0x38, 0x90 } ), e( L"elite crate") } );
						goto end;
					}
					else if ( strstr( name, e( "crate_normal.prefab" ) ) )
					{
						element = std::tuple<geo::vec3_t, const wchar_t *>( { driver->read_chain<geo::vec3_t>( object_class, { 0x30, 0x8, 0x38, 0x90 } ), e( L"military crate") } );
						goto end;
					}
					else if ( strstr( name, e( "crate_normal_2_medical.prefab" ) ) )
					{
						element = std::tuple<geo::vec3_t, const wchar_t *>( { driver->read_chain<geo::vec3_t>( object_class, { 0x30, 0x8, 0x38, 0x90 } ), e( L"medical crate") } );
						goto end;
					}
					else if ( strstr( name, e( "crate_normal_2.prefab" ) ) )
					{
						element = std::tuple<geo::vec3_t, const wchar_t *>( { driver->read_chain<geo::vec3_t>( object_class, { 0x30, 0x8, 0x38, 0x90 } ), e( L"crate") } );
						goto end;
					}
					else if ( strstr( name, e( "crate_normal_2_food.prefab" ) ) )
					{
						element = std::tuple<geo::vec3_t, const wchar_t *>( { driver->read_chain<geo::vec3_t>( object_class, { 0x30, 0x8, 0x38, 0x90 } ), e( L"food crate") } );
						goto end;
					}
					else if ( strstr( name, e( "crate_tools.prefab" ) ) )
					{
						element = std::tuple<geo::vec3_t, const wchar_t *>( { driver->read_chain<geo::vec3_t>( object_class, { 0x30, 0x8, 0x38, 0x90 } ), e( L"tool crate") } );
						goto end;
					}
				}

				if ( settings->visuals.barrels )
				{
					if ( strstr( name, e( "loot_barrel_1.prefab" ) ) )
					{
						element = std::tuple<geo::vec3_t, const wchar_t *>( { driver->read_chain<geo::vec3_t>( object_class, { 0x30, 0x8, 0x38, 0x90 } ), e( L"barrel") } );
						goto end;
					}
					else if ( strstr( name, e( "oil_barrel.prefab" ) ) )
					{
						element = std::tuple<geo::vec3_t, const wchar_t *>( { driver->read_chain<geo::vec3_t>( object_class, { 0x30, 0x8, 0x38, 0x90 } ), e( L"oil barrel") } );
						goto end;
					}
					else if ( strstr( name, e( "loot_barrel_2.prefab" ) ) )
					{
						element = std::tuple<geo::vec3_t, const wchar_t *>( { driver->read_chain<geo::vec3_t>( object_class, { 0x30, 0x8, 0x38, 0x90 } ), e( L"barrel") } );
						goto end;
					}
					else if ( strstr( name, e( "loot-barrel-1.prefab" ) ) )
					{
						element = std::tuple<geo::vec3_t, const wchar_t *>( { driver->read_chain<geo::vec3_t>( object_class, { 0x30, 0x8, 0x38, 0x90 } ), e( L"barrel") } );
						goto end;
					}
					else if ( strstr( name, e( "loot-barrel-2.prefab" ) ) )
					{
						element = std::tuple<geo::vec3_t, const wchar_t *>( { driver->read_chain<geo::vec3_t>( object_class, { 0x30, 0x8, 0x38, 0x90 } ), e( L"barrel") } );
						goto end;
					}
				}

				if ( settings->visuals.traps )
				{
					if ( strstr( name, e( "autoturret_deployed.prefab" ) ) )
					{
						element = std::tuple<geo::vec3_t, const wchar_t *>( { driver->read_chain<geo::vec3_t>( object_class, { 0x30, 0x8, 0x38, 0x90 } ), e( L"turret") } );
						goto end;
					}
					else if ( strstr( name, e( "beartrap.prefab" ) ) )
					{
						element = std::tuple<geo::vec3_t, const wchar_t *>( { driver->read_chain<geo::vec3_t>( object_class, { 0x30, 0x8, 0x38, 0x90 } ), e( L"bear trap") } );
						goto end;
					}
					else if ( strstr( name, e( "guntrap.deployed" ) ) )
					{
						element = std::tuple<geo::vec3_t, const wchar_t *>( { driver->read_chain<geo::vec3_t>( object_class, { 0x30, 0x8, 0x38, 0x90 } ), e( L"gun trap") } );
						goto end;
					}
					else if ( strstr( name, e( "flameturret.deployed.prefab" ) ) )
					{
						element = std::tuple<geo::vec3_t, const wchar_t *>( { driver->read_chain<geo::vec3_t>( object_class, { 0x30, 0x8, 0x38, 0x90 } ), e( L"flame turret") } );
						goto end;
					}
					else if ( strstr( name, e( "sam_site_turret_deployed.prefab" ) ) )
					{
						element = std::tuple<geo::vec3_t, const wchar_t *>( { driver->read_chain<geo::vec3_t>( object_class, { 0x30, 0x8, 0x38, 0x90 } ), e( L"sam-site") } );
						goto end;
					}
					else if ( strstr( name, e( "spikes.floor.prefab" ) ) )
					{
						element = std::tuple<geo::vec3_t, const wchar_t *>( { driver->read_chain<geo::vec3_t>( object_class, { 0x30, 0x8, 0x38, 0x90 } ), e( L"spikes" ) } );
						goto end;
					}
					else if ( strstr( name, e( "trap.landmine.prefab" ) ) )
					{
						element = std::tuple<geo::vec3_t, const wchar_t *>( { driver->read_chain<geo::vec3_t>( object_class, { 0x30, 0x8, 0x38, 0x90 } ), e( L"mine" ) } );
						goto end;
					}
				}

			end: 

				auto local_head_3d = cache::local_player->get_transform< c_transform *>( 47 )->position( );

				if ( local_head_3d.empty( ) )
				{
					continue;
				}

				const auto distance = std::get<0>( element ).distance( local_head_3d );

				if ( distance > settings->visuals.max_distance / 2.f )
				{
					continue;
				}

				others.push_back( element );
			}

			cache::mtx.lock( );

			cache::players = players;
			cache::others = others;

			cache::mtx.unlock( );

			Sleep( 750 );
		}
	}
}