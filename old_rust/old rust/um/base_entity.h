class c_base_entity
{
public:

	auto get_client_entities( ) -> c_list<std::uintptr_t>*
	{
		return driver->read_chain<c_list<std::uintptr_t>*>( reinterpret_cast< std::uintptr_t >( this ), { 0x10, 0x10, 0x28 } );
	}
};