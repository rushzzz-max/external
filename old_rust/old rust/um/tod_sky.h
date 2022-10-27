class c_tod_ambient
{
public:

	auto set_update_interval( const float interval ) -> void
	{
		driver->write<float>( reinterpret_cast< std::uintptr_t >( this ) + m_update_interval, interval );
	}
};

class c_tod_night
{
public:

	auto set_ambient_multiplier( const float multiplier ) -> void
	{
		driver->write<float>( reinterpret_cast< std::uintptr_t >( this ) + m_ambient_multiplier, multiplier );
	}

	auto set_shadow_strength( const float strength ) -> void
	{
		driver->write<float>( reinterpret_cast< std::uintptr_t >( this ) + m_shadow_strength, strength );
	}
};

class c_tod_day
{
public:

	auto set_ambient_multiplier( const float multiplier ) -> void
	{
		driver->write<float>( reinterpret_cast< std::uintptr_t >( this ) + m_ambient_multiplier, multiplier );
	}
};

class c_tod_cycle
{
public:

	auto set_hour( const float hour ) -> void
	{
		driver->write<std::uintptr_t>( reinterpret_cast< std::uintptr_t >( this ) + m_hour, hour );
	}
};

class c_tod_scattering
{
public:

	auto get_material( ) -> const std::uintptr_t
	{
		return driver->read<std::uintptr_t>( reinterpret_cast< std::uintptr_t >( this ) + m_material );
	}
};

class c_tod_components
{
public:

	auto get_scattering( ) -> c_tod_scattering *
	{
		return driver->read<c_tod_scattering *>( reinterpret_cast< std::uintptr_t >( this ) + m_scattering );
	}
};

class c_tod_sky
{
public:

	auto get_tod_aimbient( ) -> c_tod_ambient *
	{
		return driver->read< c_tod_ambient *>( reinterpret_cast< std::uintptr_t >( this ) + m_aimbient );
	}

	auto get_tod_night( ) -> c_tod_night *
	{
		return driver->read< c_tod_night *>( reinterpret_cast< std::uintptr_t >( this ) + m_night );
	}

	auto get_tod_cycle( ) -> c_tod_cycle *
	{
		return driver->read< c_tod_cycle *>( reinterpret_cast< std::uintptr_t >( this ) + m_cycle );
	}

	auto get_tod_components( ) -> c_tod_components *
	{
		return driver->read<c_tod_components *>( reinterpret_cast< std::uintptr_t >( this ) + m_components );
	}
	auto get_tod_day( ) -> c_tod_day *
	{
		return driver->read<c_tod_day *>( reinterpret_cast< std::uintptr_t >( this ) + m_day );
	}
};