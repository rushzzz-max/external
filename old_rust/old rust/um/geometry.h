namespace geo
{
	struct mat4x4_t
	{
		mat4x4_t( )
			: m { { 0, 0, 0, 0 },
				 { 0, 0, 0, 0 },
				 { 0, 0, 0, 0 },
				 { 0, 0, 0, 0 } }
		{}

		mat4x4_t( const mat4x4_t & ) = default;

		union {
			struct {
				float _11, _12, _13, _14;
				float _21, _22, _23, _24;
				float _31, _32, _33, _34;
				float _41, _42, _43, _44;
			};
			float m[4][4];
		};
	};

	class vec2_t
	{
	public:
		float x;
		float y;

		vec2_t( )
		{}

		vec2_t( const float x, const float y )
			: x( x ), y( y )
		{}

		inline auto distance( const geo::vec2_t &input ) -> float
		{
			return sqrt( pow( x - input.x, 2 ) + pow( y - input.y, 2 ) );
		}
	};

	class vec3_t
	{
	public:
		float x;
		float y;
		float z;

		vec3_t( )
		{}

		vec3_t( const float x, const float y, const float z )
			: x( x ), y( y ), z( z )
		{}

		inline auto distance( const geo::vec3_t &input ) -> float
		{
			return sqrt( pow( x - input.x, 2 ) + pow( y - input.y, 2 ) + pow( z - input.z, 2 ) );
		}

		inline auto operator+( const geo::vec3_t &v ) -> const geo::vec3_t
		{
			return geo::vec3_t( x + v.x, y + v.y, z + v.z );
		}

		inline auto operator+=( const geo::vec3_t &v ) -> const geo::vec3_t
		{
			return geo::vec3_t( x + v.x, y + v.y, z + v.z );
		}

		inline auto operator*( const float v ) -> const geo::vec3_t
		{
			return geo::vec3_t( x * v, y * v, z * v );
		}

		inline auto operator*( const geo::vec3_t &v ) -> const geo::vec3_t
		{
			return geo::vec3_t( x * v.x, y * v.y, z * v.z );
		}

		inline auto operator-( const geo::vec3_t &v ) -> const geo::vec3_t
		{
			return geo::vec3_t( x - v.x, y - v.y, z - v.z );
		}

		inline auto operator/( float v ) -> geo::vec3_t
		{
			return geo::vec3_t( x / v, y / v, z / v );
		}

		inline auto length( ) -> float
		{
			return sqrtf( x * x + y * y + z * z );
		}

		inline auto empty( ) -> bool
		{
			return ( x == 0.f && y == 0.f && z == 0.f ) || ( x == -1.f && y == -1.f && z == -1.f );
		}
	};

	class vec4_t
	{
	public:
		float x;
		float y;
		float z;
		float w;

		vec4_t( )
		{}

		vec4_t( float x, float y, float z, float w )
			: x( x ), y( y ), z( z ), w( w )
		{}

		inline auto distance( const geo::vec4_t &input ) -> float
		{
			return sqrt( pow( x - input.x, 2 ) + pow( y - input.y, 2 ) + pow( z - input.z, 2 ) + pow( w - input.w, 2 ) );
		}

		inline auto operator+( const geo::vec4_t &v ) -> const geo::vec4_t
		{
			return geo::vec4_t( x + v.x, y + v.y, z + v.z, w + v.w );
		}

		inline auto operator+=( const geo::vec4_t &v ) -> const geo::vec4_t
		{
			return geo::vec4_t( x + v.x, y + v.y, z + v.z, w + v.w );
		}

		inline auto operator*( const float v ) -> const geo::vec4_t
		{
			return geo::vec4_t( x * v, y * v, z * v, w * v );
		}

		inline auto operator*( const geo::vec4_t &v ) -> const geo::vec4_t
		{
			return geo::vec4_t( x * v.x, y * v.y, z * v.z, w * v.w );
		}

		inline auto operator-( const geo::vec4_t &v ) -> const geo::vec4_t
		{
			return geo::vec4_t( x - v.x, y - v.y, z - v.z, w - v.w );
		}
	};

	auto dot( const geo::vec3_t &_1, const geo::vec3_t &_2 ) -> float
	{
		return _1.x * _2.x + _1.y * _2.y + _1.z * _2.z;
	}

	auto calculate_angle( geo::vec3_t &src, const geo::vec3_t &dst ) -> geo::vec2_t
	{
		auto direction = src - dst;

		return
		{
			( ( float )( ( asin( direction.y / direction.length( ) ) ) ) * ( float )( 180.f / 3.14159265358979323846f ) ),
			( ( float )( -atan2( direction.x, -direction.z ) ) * ( float )( 180.f / 3.14159265358979323846f ) )
		};
	}

	auto to_quaternion( geo::vec2_t& euler ) -> geo::vec4_t
	{
		const auto to_radians = [&]( float degree ) -> double
		{
			double pi = 3.14159265359f;
			return ( degree * ( pi / 180.f ) );
		};

		float heading = to_radians( euler.x );
		float attitude = to_radians( euler.y );
		float bank = to_radians( 0 );

		float c1 = cos( heading / 2 );
		float s1 = sin( heading / 2 );
		float c2 = cos( attitude / 2 );
		float s2 = sin( attitude / 2 );
		float c3 = cos( bank / 2 );
		float s3 = sin( bank / 2 );
		float c1c2 = c1 * c2;
		float s1s2 = s1 * s2;
		geo::vec4_t quat { c1c2 * s3 + s1s2 * c3, s1 * c2 * c3 + c1 * s2 * s3, c1 * s2 * c3 - s1 * c2 * s3, c1c2 * c3 - s1s2 * s3 };
		return { quat.y, quat.z, ( quat.x * -1 ), quat.w };
	}

	auto clamp( float &yaw, float &pitch ) -> void
	{
		if ( yaw > 89.0f )
		{
			yaw = 89.0f;
		}
		if ( yaw < -89.0f )
		{
			yaw = -89.0f;
		}
		while ( pitch > 180.0f )
		{
			pitch -= 360.0f;
		}
		while ( pitch < -180.0f )
		{
			pitch += 360.0f;
		}
	}

	bool in_square( geo::vec2_t point, geo::vec2_t position, int size )
	{
		return !( point.x < position.x || point.y < position.y || point.x > position.x + ( float )size || point.y > position.y + ( float )size );
	}
}