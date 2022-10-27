#define RGB(r, g, b) ( ( COLORREF )( ( ( BYTE )( r ) | ( ( WORD )( ( BYTE )( g ) ) << 8 ) ) | ( ( ( DWORD )( BYTE )( b ) ) << 16 ) ) )
#define rva(ptr, size) ((DWORD64)ptr + size + *(LONG*)((DWORD64)ptr + (size - sizeof(LONG))))
#define offset(ptr, size) ( ptr + size + *( int32_t *)( ptr + size ) + sizeof( int32_t ) )

namespace win32k
{
	int( *release_dc_fn ) ( HDC ) = nullptr;
	bool ( *delete_object_app_fn )( HANDLE ) = nullptr;
	HBRUSH( *nt_gdi_create_solid_brush_fn )( COLORREF, HBRUSH ) = nullptr;
	HDC( *nt_user_get_dc_ex_fn )( HWND, HANDLE, ULONG ) = nullptr;
	bool ( *pat_blt_fn )( HDC, int, int, int, int, DWORD ) = nullptr;
	HBRUSH( *gdi_select_brush_fn )( HDC, HBRUSH ) = nullptr;
	void( *gre_ext_text_out_w_internal_fn )( HDC, int, int, uintptr_t, uintptr_t, LPCWSTR, int, uintptr_t, uintptr_t ) = nullptr;
	COLORREF( *gre_set_text_color_fn )( HDC, COLORREF ) = nullptr;
	COLORREF( *gre_set_bk_color_fn )( HDC, COLORREF ) = nullptr;
	UINT( *gre_set_text_align_fn )( HDC, UINT ) = nullptr;
	HWND( *nt_user_get_foreground_window_fn )( ) = nullptr;
	int( *nt_user_query_window_fn )( HWND, int ) = nullptr;

	auto initialize( ) -> void
	{
		const auto win32kbase = utils::get_kernel_module( e( "win32kbase.sys" ) );
		const auto win32kfull = utils::get_kernel_module( e( "win32kfull.sys" ) );

		nt_user_get_foreground_window_fn = utils::get_module_export<HWND( * )( )>( win32kfull, e( "NtUserGetForegroundWindow" ) );
		nt_user_query_window_fn = utils::get_module_export<int( * )( HWND, int ) >( win32kfull, e( "NtUserQueryWindow" ) );
		release_dc_fn = utils::get_module_export<int( * )( HDC )>( win32kbase, e( "NtUserReleaseDC" ) );
		delete_object_app_fn = utils::get_module_export<bool ( * )( HANDLE )>( win32kbase, e( "NtGdiDeleteObjectApp" ) );
		nt_user_get_dc_ex_fn = utils::get_module_export<HDC( * )( HWND, HANDLE, ULONG )>( win32kfull, e( "NtUserGetDCEx" ) );
		pat_blt_fn = utils::get_module_export<bool( * )( HDC, int, int, int, int, DWORD )>( win32kfull, e( "NtGdiPatBlt" ) );
		nt_gdi_create_solid_brush_fn = utils::get_module_export<HBRUSH( * )( COLORREF, HBRUSH )>( win32kfull, e( "NtGdiCreateSolidBrush" ) );
		gdi_select_brush_fn = utils::get_module_export<HBRUSH( * )( HDC, HBRUSH )>( win32kfull, e( "NtGdiSelectBrush" ) );

		gre_ext_text_out_w_internal_fn = reinterpret_cast< void( * )( HDC, int, int, uintptr_t, uintptr_t, LPCWSTR, int, uintptr_t, uintptr_t ) > (
			rva( utils::find_pattern( win32kfull, e( "\xE8\x00\x00\x00\x00\x49\x83\xC6\x00\xBB" ), e( "x????xxx?x" ) ), 5 ) );

		gre_set_text_color_fn = reinterpret_cast< COLORREF( * )( HDC, COLORREF ) >(
			offset( utils::find_pattern( win32kfull, e( "\x48\x8B\xCF\xE8\x00\x00\x00\x00\x44\x8B\xF8\x45\x33\xC9" ), e( "xxxx????xxxxxx" ) ), 4 ) );

		gre_set_bk_color_fn = reinterpret_cast< COLORREF( * )( HDC, COLORREF ) >(
			offset( utils::find_pattern( win32kfull, e( "\xE8\x00\x00\x00\x00\xBA\x00\x00\x00\x00\x48\x8B\xCB\x44\x8B\xF0\xE8" ), e( "x????x????xxxxxxx" ) ), 17 ) );

		gre_set_text_align_fn = reinterpret_cast< UINT( * )( HDC, UINT ) >(
			rva( utils::find_pattern( win32kfull, e( "\xE8\x00\x00\x00\x00\x89\x5C\x24\x48\x45" ), e( "x????xxxxx" ) ), 5 ) );
	}

	auto draw_rect( uint32_t x, uint32_t y, uint32_t w, uint32_t h, uint8_t t, uint8_t r, uint8_t g, uint8_t b ) -> bool
	{
		const auto hdc = win32k::nt_user_get_dc_ex_fn( 0, 0, 1 );

		if ( !hdc )
		{
			return false;
		}

		const auto brush = nt_gdi_create_solid_brush_fn( RGB( r, g, b ), 0 );

		if ( !brush )
		{
			release_dc_fn( hdc );
			return false;
		}

		const auto old_brush = gdi_select_brush_fn( hdc, brush );

		if ( !old_brush )
		{
			delete_object_app_fn( brush );
			release_dc_fn( hdc );
			return false;
		}

		pat_blt_fn( hdc, x, y, t, y + h - y, 0x00F00021 );
		pat_blt_fn( hdc, x + w - t, y, t, y + h - y, 0x00F00021 );
		pat_blt_fn( hdc, x, y, x + w - x, t, 0x00F00021 );
		pat_blt_fn( hdc, x, y + h - t, x + w - x, t, 0x00F00021 );

		gdi_select_brush_fn( hdc, old_brush );

		delete_object_app_fn( brush );

		release_dc_fn( hdc );

		return true;
	}

	auto draw_text( uint32_t x, uint32_t y, const wchar_t *wstr, uint8_t r, uint8_t g, uint8_t b ) -> bool
	{
		const auto hdc = nt_user_get_dc_ex_fn( 0, 0, 1 );

		if ( !hdc )
		{
			return false;
		}

		const auto old_text_color = gre_set_text_color_fn( hdc, RGB( r, g, b ) );

		const auto old_bk_color = gre_set_bk_color_fn( hdc, RGB( 1, 1, 1 ) );

		const auto old_text_align = gre_set_text_align_fn( hdc, 6 );

		gre_ext_text_out_w_internal_fn( hdc, x, y, 0, 0, wstr, crt::strlen( wstr ), 0, 0 );

		gre_set_text_color_fn( hdc, old_text_color );

		gre_set_bk_color_fn( hdc, old_bk_color );

		gre_set_text_align_fn( hdc, old_text_align );

		release_dc_fn( hdc );

		return true;
	}
}