#include <windows.h>
#include <iostream>
#include <vector>
#include <emmintrin.h>
#include <thread>
#include <string>
#include <array>
#include <map>
#include <mutex>

#include "settings.h"
#include "encrypt.h"
#include "globals.h"
#include "offsets.h"
#include "driver.h"
#include "system.h"
#include "skinnable.h"
#include "tod_sky.h"
#include "convar_client.h"
#include "base_entity.h"
#include "game_assembly.h"
#include "geometry.h"
#include "transform.h"
#include "player.h"
#include "camera.h"
#include "unity.h"
#include "cache.h"
#include "aim.h"
#include "features.h"
#include "menu.h"
#include "local.h"
#include "visuals.h"
#include "core.h"

void main( )
{
	SetPriorityClass( GetCurrentProcess( ), REALTIME_PRIORITY_CLASS );

	globals::game.hwnd = FindWindow( e( L"UnityWndClass" ), e( L"Rust" ) );

	globals::game.tid = GetWindowThreadProcessId( globals::game.hwnd, &globals::game.pid );

	driver->initialize( globals::game.pid );

	const auto on_update = [] ( ) -> void
	{
		globals::game.hwnd = FindWindow( e( L"UnityWndClass" ), e( L"Rust" ) );

		RECT rect { 0, 0, 0, 0 };
		GetClientRect( globals::game.hwnd, &rect );

		globals::game.w = rect.right;
		globals::game.h = rect.bottom;
		printf( "globals::game.w: %i\n", globals::game.w );
		printf( "globals::game.h: %i\n\n\n", globals::game.h );

		POINT screen = { 0, 0 };
		ClientToScreen( globals::game.hwnd, &screen );

		globals::game.x = screen.x;
		globals::game.y = screen.y;
		printf( "globals::game.x: %i\n", globals::game.x );
		printf( "globals::game.y: %i\n\n\n", globals::game.y );

		globals::game.shown = ( GetForegroundWindow( ) == globals::game.hwnd );
	};

	while ( true )
	{
		driver->draw_rect( 50, 50, 50, 50, 1, 255, 0, 0 );
		printf( "sent\n" );
	}
	std::thread( [&]( ) { core::entity_loop( on_update ); } ).detach( );
	std::thread( [&]( ) { local::self_loop( ); } ).detach( );
	std::thread( [&]( ) { local::fast_loop( ); } ).detach( );
	
	visuals::draw_loop( );
}