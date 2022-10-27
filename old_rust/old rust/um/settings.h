class c_settings
{
public:

	struct
	{
		bool players = true;
		bool loots = false;
		bool ores = false;
		bool vehicules = false;
		bool crates = false;
		bool barrels = false;
		bool collectables = false;
		bool traps = false;
		int max_distance = 350;
	}visuals;

	struct
	{
		bool silent_aim = true;
		int fov = 150;
		bool blatant = true;
		bool team_check = true;
		int max_distance = 250;
	}aim;

	struct
	{
		bool no_spread = true;
		bool no_recoil = true;
		bool no_sway = true;
		bool super_eoka = false;
		bool fat_bullet = false;
	}weapon;

	struct
	{
		bool fake_admin = true;
		bool night_mode = true;
		bool shoot_in_copter = true;
		bool chams = true;
	}misc;

	struct
	{
		bool speedhack = false;
		bool infinite_jump = true;
		bool spiderman = true;
		bool onmisprint = true;
	}movement;
};

static c_settings *settings = new c_settings( );