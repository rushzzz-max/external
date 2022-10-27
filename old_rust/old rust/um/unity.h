class c_unity
{
public:

	auto get_camera( ) -> c_camera *
	{
		return driver->read_chain<c_camera *>( reinterpret_cast<std::uintptr_t>( this ), { m_camera_manager, 0x0, 0x0, 0x30, 0x30, 0x18 } );
	}
};