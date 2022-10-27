template <typename t>
class c_list
{
public:

	auto size( ) -> const std::uint32_t
	{
		return driver->read<std::uint32_t>( reinterpret_cast< std::uintptr_t >( this ) + 0x10 );
	}

	auto get( const std::uint32_t idx ) -> t
	{
		return driver->read_chain<t>( reinterpret_cast< std::uintptr_t >( this ), { 0x18, 0x20ull + ( idx * 0x8ull ) } );
	}
};

template <typename t>
class c_array
{
public:

	auto size( ) -> const std::uint32_t
	{
		return driver->read<std::uint32_t>( reinterpret_cast< std::uintptr_t >( this ) + 0x18 );
	}

	auto get( const std::uint32_t idx ) -> t
	{
		return driver->read<t>( reinterpret_cast< std::uintptr_t >( this ) + 0x20ull + ( idx * 0x8ull ) );
	}
};

enum class category
{
	shirt = 0,
	pants = 5,
	jacket = 1,
	hat = 2,
	mask = 3,
	footwear = 4,
	weapon = 6,
	misc = 7,
	deployable = 9,
	gloves = 10,
};