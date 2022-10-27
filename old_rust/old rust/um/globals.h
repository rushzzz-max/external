namespace globals
{
	struct
	{
		unsigned int x;
		unsigned int y;
		unsigned int w;
		unsigned int h;
		unsigned long pid;
		unsigned long tid;
		HWND hwnd;
		bool shown;
	}game;
}

struct unity_str
{
	char buffer[128];
};