#pragma once
namespace utils
{
	class locations
	{
	public:
		static std::string GetCurrentWorkingDir();
	};

	class DLL_Management
	{
	public:
		static void Kill_DLL();
	};
}