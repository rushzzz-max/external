class c_convar_client
{
public:

	auto set_camspeed( const float speed ) -> void
	{
		driver->write<float>( reinterpret_cast< std::uintptr_t >( this ) + m_camspeed, speed );
	}

	auto set_camlerp( const float lerp ) -> void
	{
		driver->write<float>( reinterpret_cast< std::uintptr_t >( this ) + m_camlerp, lerp );
	}

	auto set_camlookspeed( const float speed ) -> void
	{
		driver->write<float>( reinterpret_cast< std::uintptr_t >( this ) + m_camlookspeed, speed );
	}
};