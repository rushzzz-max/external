namespace menu
{
	int frames = 0;
	bool draw = 0;
	int selected = 1;

	auto add_toggle( const char *name, bool *value ) -> void
	{
		if ( !draw )
		{
			return;
		}

		std::string str = ( selected == frames ? e( ">" ) :e(  "" ) );
		str += name;
		str += e( " [" );
		str += ( *value ? e( "ON]" ) : e( "OFF]" ) );
		str += ( selected == frames ? e( "<" ) : e( "" ) );

		driver->draw_text( 140, 40 + ( frames * 17 ),
			std::wstring( str.begin( ), str.end( )).c_str(),
			(*value ? 0 : 255 ), ( *value ? 255 : 0 ), 0 );

		if ( selected == frames && ( GetAsyncKeyState( VK_RIGHT ) & 1 ) )
		{
			*value = true;
		}
		else if ( selected == frames && ( GetAsyncKeyState( VK_LEFT ) & 1 ) )
		{
			*value = false;
		}

		frames++;
	}

	auto add_slider( const char *name, int *value, int min, int max, int add = 4 ) -> bool
	{
		if ( !draw )
		{
			return false;
		}

		std::string str = ( selected == frames ? e( ">" ) : e( "" ) );
		str += name;
		str += e( " [" );
		str += std::to_string( *value );
		str += e( "]" );
		str += ( selected == frames ? e( "<" ) : e( "" ) );

		driver->draw_text( 140, 40 + ( frames * 17 ),
			std::wstring( str.begin( ), str.end( ) ).c_str( ),
			( *value ? 0 : 255 ), ( *value ? 255 : 0 ), 0 );

		bool held = false;
		if ( selected == frames && *value < max && *value > min && ( GetAsyncKeyState( VK_RIGHT ) & 1 ) )
		{
			held = true;
			*value += add;
		}
		else if ( selected == frames && *value < max && *value > min && ( GetAsyncKeyState( VK_LEFT ) & 1 ) )
		{
			held = true;
			*value -= add;
		}

		frames++;
		return held;
	}

	auto add_title( const char *title ) -> void
	{
		if ( !draw )
		{
			return;
		}

		std::string str = e( "[=" );
		str += title;
		str += e( "=]" );
		frames++;
		driver->draw_text( 140, 40 + ( frames * 17 ), std::wstring( str.begin( ), str.end( ) ).c_str( ), 238, 0, 255 );
		frames++;
	}

	void reset( )
	{
		frames = 0;

		if ( GetAsyncKeyState( VK_F2 ) & 1 )
		{
			draw = !draw;
		}

		if ( draw )
		{
			if ( ( GetAsyncKeyState( VK_DOWN ) & 1 ) )
			{
				selected++;
			}
			else if ( ( GetAsyncKeyState( VK_UP ) & 1 ) && selected > 1 )
			{
				selected--;
			}
		}
	}
}