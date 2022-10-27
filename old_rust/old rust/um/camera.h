class c_camera
{
public:
	auto get_view_matrix( ) -> geo::mat4x4_t
	{
		return driver->read<geo::mat4x4_t>( reinterpret_cast< std::uintptr_t > ( this ) + 0x2E4 );
	}

	auto world_to_screen( const geo::mat4x4_t &matrix, const geo::vec3_t &position ) -> geo::vec3_t
	{
		const auto w = dot( { matrix._14, matrix._24, matrix._34 }, position ) + matrix._44;

		if ( w < 0.097f )
		{
			return { 0.f, 0.f, 0.f };
		}

		return
		{
			( ( float )globals::game.w / 2.f ) * ( 1.f + ( dot( { matrix._11, matrix._21, matrix._31 }, position ) + matrix._41 ) / w ),
			( ( float )globals::game.h / 2.f ) * ( 1.f - ( dot( { matrix._12, matrix._22, matrix._32 }, position ) + matrix._42 ) / w ), 1
		};
	}
};