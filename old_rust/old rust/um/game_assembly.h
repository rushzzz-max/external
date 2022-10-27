class c_game_assembly
{
public:

	auto get_base_entity( ) -> c_base_entity *
	{
		return driver->read_chain<c_base_entity *>( reinterpret_cast< std::uintptr_t >( this ), { m_base_entity_c, 0xB8 } );
	}

	auto get_convar_client( ) -> c_convar_client *
	{
		return driver->read_chain<c_convar_client *>( reinterpret_cast< std::uintptr_t >( this ), { m_convar_client_c, 0xB8 } );
	}

	auto get_skinnable( ) -> c_skinnable *
	{
		return driver->read_chain<c_skinnable *>( reinterpret_cast< std::uintptr_t >( this ), { m_skinnable_c, 0xB8 } );
	}

	auto get_tod_sky( ) -> c_tod_sky *
	{
		return driver->read_chain<c_tod_sky *>( reinterpret_cast< std::uintptr_t >( this ), { m_tod_sky_c, 0xB8, 0x0, 0x10, 0x20 } );
	}
};