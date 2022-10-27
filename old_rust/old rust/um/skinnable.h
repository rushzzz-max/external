class c_groups
{
public:

	auto set_material( const std::uintptr_t material ) -> void
	{
		driver->write<std::uintptr_t>( reinterpret_cast< std::uintptr_t >( this ) + m_skinnable_material, material );
	}
};

class c_skinnable
{
public:

	auto get_category( ) -> category
	{
		return driver->read<category>( reinterpret_cast< std::uintptr_t >( this ) + m_category );
	}

	auto get_groups( ) -> c_array<c_groups *> *
	{
		return driver->read< c_array<c_groups *> *>( reinterpret_cast< std::uintptr_t >( this ) + m_groups );
	}

	auto get_all( ) -> c_array<c_skinnable *> *
	{
		return driver->read< c_array<c_skinnable *> *>( reinterpret_cast< std::uintptr_t >( this ) + 0x0 );
	}
};