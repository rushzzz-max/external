class c_transform
{
private:
	struct transform_access_read_only_t
	{
		std::uintptr_t data;
	};

	struct transform_data_t
	{
		std::uintptr_t array;
		std::uintptr_t indices;
	};

	struct matrix34_t
	{
		geo::vec4_t _0;
		geo::vec4_t _1;
		geo::vec4_t _2;
	};

public:

	auto position( ) -> geo::vec3_t
	{
		if ( !reinterpret_cast< std::uintptr_t >( this ) )
		{
			return { 0.f, 0.f, 0.f };
		}

		__m128 result;

		const __m128 mulVec0 = { -2.000, 2.000, -2.000, 0.000 };
		const __m128 mulVec1 = { 2.000, -2.000, -2.000, 0.000 };
		const __m128 mulVec2 = { -2.000, -2.000, 2.000, 0.000 };

		transform_access_read_only_t transform_access_read_only = driver->read< transform_access_read_only_t > ( reinterpret_cast< std::uintptr_t >( this ) + 0x38 );

		unsigned int index = driver->read<unsigned int>( reinterpret_cast< std::uintptr_t >( this ) + 0x40 );

		const auto transform_data = driver->read<transform_data_t>( transform_access_read_only.data + 0x18 );

		if ( transform_data.array && transform_data.indices )
		{
			result = driver->read<__m128>( transform_data.array + 0x30 * index );
			
			int transform_index = driver->read<int>( transform_data.indices + 0x4 * index );
			
			int safe = 0;

			while ( transform_index >= 0 && safe++ < 200 )
			{
				matrix34_t matrix34 = driver->read<matrix34_t>( transform_data.array + 0x30 * transform_index );

				__m128 xxxx = _mm_castsi128_ps( _mm_shuffle_epi32( *( __m128i * )( &matrix34._1 ), 0x00 ) );	// xxxx
				__m128 yyyy = _mm_castsi128_ps( _mm_shuffle_epi32( *( __m128i * )( &matrix34._1 ), 0x55 ) );	// yyyy
				__m128 zwxy = _mm_castsi128_ps( _mm_shuffle_epi32( *( __m128i * )( &matrix34._1 ), 0x8E ) );	// zwxy
				__m128 wzyw = _mm_castsi128_ps( _mm_shuffle_epi32( *( __m128i * )( &matrix34._1 ), 0xDB ) );	// wzyw
				__m128 zzzz = _mm_castsi128_ps( _mm_shuffle_epi32( *( __m128i * )( &matrix34._1 ), 0xAA ) );	// zzzz
				__m128 yxwy = _mm_castsi128_ps( _mm_shuffle_epi32( *( __m128i * )( &matrix34._1 ), 0x71 ) );	// yxwy
				__m128 tmp7 = _mm_mul_ps( *( __m128 * )( &matrix34._2 ), result );

				result = _mm_add_ps(
					_mm_add_ps(
						_mm_add_ps(
							_mm_mul_ps(
								_mm_sub_ps(
									_mm_mul_ps( _mm_mul_ps( xxxx, mulVec1 ), zwxy ),
									_mm_mul_ps( _mm_mul_ps( yyyy, mulVec2 ), wzyw ) ),
								_mm_castsi128_ps( _mm_shuffle_epi32( _mm_castps_si128( tmp7 ), 0xAA ) ) ),
							_mm_mul_ps(
								_mm_sub_ps(
									_mm_mul_ps( _mm_mul_ps( zzzz, mulVec2 ), wzyw ),
									_mm_mul_ps( _mm_mul_ps( xxxx, mulVec0 ), yxwy ) ),
								_mm_castsi128_ps( _mm_shuffle_epi32( _mm_castps_si128( tmp7 ), 0x55 ) ) ) ),
						_mm_add_ps(
							_mm_mul_ps(
								_mm_sub_ps(
									_mm_mul_ps( _mm_mul_ps( yyyy, mulVec0 ), yxwy ),
									_mm_mul_ps( _mm_mul_ps( zzzz, mulVec1 ), zwxy ) ),
								_mm_castsi128_ps( _mm_shuffle_epi32( _mm_castps_si128( tmp7 ), 0x00 ) ) ),
							tmp7 ) ), *( __m128 * )( &matrix34._0 ) );

				transform_index = driver->read<int>( transform_data.indices + 0x4 * transform_index );
			}
		}

		return geo::vec3_t( result.m128_f32[0], result.m128_f32[1], result.m128_f32[2] );
	}
};