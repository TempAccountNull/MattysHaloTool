#pragma once
namespace utils
{
	class locations
	{
	public:
		static std::string get_current_working_dir();
	};

	class dll_management
	{
	public:
		static void kill_dll();
	};
}