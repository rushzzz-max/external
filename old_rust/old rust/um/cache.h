namespace cache
{
	c_game_assembly *game_assembly = nullptr;
	c_unity *unity = nullptr;
	c_skinnable *skinnable = nullptr;
	c_tod_sky* tod_sky = nullptr;
	c_camera *camera = nullptr;
	c_player *local_player = nullptr;
	c_player *targeted = nullptr;
	
	std::mutex mtx;
	std::vector < std::tuple < c_player *, c_model*, bool, std::wstring > > players = { };
	std::vector<std::tuple<geo::vec3_t, const wchar_t *>> others;
}